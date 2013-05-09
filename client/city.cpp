#include "city.h"

#include <iostream>
#include <sstream>

#include "Region.h"
#include "server.h"

#include "sc4savegame.h"
#include "regionviewfile.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QCryptographicHash>

using namespace std;

QString md5(const QByteArray& data)
{
    return QString(QCryptographicHash::hash(data,QCryptographicHash::Md5));
}


City::City(Region *region, Json::Value& data) :
    QObject((QObject*)region)
{
    m_Upload = 0;
    m_Download = 0;
    m_TransferPercen =0;
    m_Modified = false;

    cout << "Parsing city: " << data.toStyledString() << endl;

    updateFromJson(data);
}

City::City(Region *region,const QString& path) :
    QObject((QObject*)region)
{
    m_Upload = 0;
    m_Download = 0;
    m_TransferPercen =0;
    m_Modified = false;

    updatedBaseMd5();
    updateFromFile(path);
}

void City::updatedBaseMd5()
{
    QFile file(baseFilename());

    if(!file.exists())
        return;

    file.open(QFile::ReadOnly);
    uchar *memory = file.map(0, file.size());
    if (memory)
        return;

    QByteArray fileData = QByteArray::fromRawData((char*)memory,file.size());

    m_LiveMd5 =  md5(fileData);

    file.unmap(memory);
    file.close();
}

void City::updateFromJson(const Json::Value& data)
{

    qDebug() << "City Json:" << data.toStyledString().c_str();

    QString newHeadMd5 = QString::fromStdString( data["md5hash"].asString() );

    if(m_HeadMd5 != "" && m_HeadMd5 != newHeadMd5)
    {
        qDebug() << "server md5 is different, city out of date";
        startDownload();

        //m_headMd5 will be writen when download finishes
    }

    m_Name = QString::fromStdString( data["name"].asString() );
    m_Owner = QString::fromStdString( data["mayorName"].asString() );
    m_Guid = QString::fromStdString( data["guid"].asString() );

    m_Position = QPoint( data["tileX"].asInt(), data["tileY"].asInt());
    m_Size = data["sizeX"].asInt();

}

void City::updateFromFile(const QString &path)
{
    qDebug() << "Update city '" << m_Name << "' from file: " << path;



    QFile file(path);

    file.open(QFile::ReadOnly);
    uchar *memory = file.map(0, file.size());
    if (memory)
        return;

    QByteArray fileData = QByteArray::fromRawData((char*)memory,file.size());

    m_LiveMd5 =  md5(fileData);

    file.unmap(memory);

    if(m_HeadMd5 !=  "" && m_LiveMd5 != m_HeadMd5)
    {
        qDebug() << "Detcted lcoal changes, uploading city: " << name();
        startUpload(m_LiveMd5,m_HeadMd5);
    }

    Sc4SaveGame save(path.toStdString());

    RegionViewFile* regionData = RegionViewFile::loadFrom(save);

    cout << "City " << regionData->cityName << " guid " << hex << regionData->guid << endl;

    m_Name = QString::fromStdString(regionData->cityName);
    m_Guid = QString::number(regionData->guid,16);
    m_Filename = path;

    m_Position = QPoint(regionData->tileX, regionData->tileY);
    m_Size = regionData->sizeX;

    const void* thumbData;
    size_t thumbSize;

    if(!save.getFile(Sc4SaveGame::PNG,0,0,thumbData,thumbSize))
    {
        qDebug() << "Unable to get thumbnail";
        return;
    }

    m_Thumbnail.loadFromData((uchar*)thumbData,thumbSize,"png");

    delete regionData;

    qDebug() << "City updated";
    emit cityUpdated();
}


QString City::extractGuid(QString path)
{
    Sc4SaveGame save(path.toStdString());
    auto_ptr<RegionViewFile> regionData(RegionViewFile::loadFrom(save));

    return QString::number(regionData->guid,16);
}

City::~City()
{

}

void City::prepareForPlay()
{
    if(m_Filename.length()==0)
    {
        if(!isLoading())
            startDownload();
    }
    else
    {
        //Check the hash and download if stale


    }
}

void City::determineNewFilename()
{
    QDir regionDir = region()->saveDirectory();
    QString proposedName = QString("City - %1.sc4").arg(name());
    int i = 1;

    while(m_Filename.length() == 0)
    {

        if(!regionDir.exists(proposedName))
        {
            m_Filename = regionDir.absoluteFilePath(proposedName);
        }
        else
        {
            proposedName = QString("City - %1 (%2).sc4").arg(name()).arg(i);
            i++;
        }
    }

}

bool City::isLoading()
{
    return m_Download != 0;
}

void City::startDownload()
{
    cout << "Starting download of city: " << name().toStdString() << endl;

    QNetworkRequest downloadRequest;//(QUrl(url()));

    downloadRequest.setUrl(url());

    //downloadRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/maxis-sc4"));

    m_Download = region()->server()->get(downloadRequest);

    connect(m_Download,SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(transferProgress(qint64,qint64)));
    connect(m_Download,SIGNAL(finished()),this,SLOT(downloadCompleted()));

    emit cityUpdated();
}

void City::transferProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int totalSize = m_Download->header(QNetworkRequest::ContentLengthHeader).toInt();

    m_TransferPercen = bytesReceived / (float)totalSize;
    emit cityUpdated();
}

void City::downloadCompleted()
{
    QByteArray data = m_Download->readAll();

    QFile file(baseFilename());
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();

    determineNewFilename();

    QFile file2(currentFilename());
    file2.open(QFile::WriteOnly);
    file2.write(data);
    file2.close();

    updatedBaseMd5();

    m_Download->deleteLater();
    m_Download = 0;

    emit cityUpdated();
}

void City::startUpload(QString oldHash, QString newHash)
{
    qDebug() << "Uploading city " << m_Guid;

    QNetworkRequest uploadRequest;
    uploadRequest.setUrl(url());

    //TODO: bsdiff this for large uploads

    QFile file(m_Filename);
    file.open(QFile::ReadOnly);

    m_Upload = region()->server()->post(uploadRequest, file);

    connect(m_Download,SIGNAL(uploadProgress(qint64,qint64)),this, SLOT(transferProgress(qint64,qint64)));
    connect(m_Download,SIGNAL(finished()),this,SLOT(uploadCompleted()));

    file.close();

    emit cityUpdated();
}

void City::uploadCompleted()
{
    if(m_Upload->error() == QNetworkReply::NoError)
    {
        m_Modified = false;
        QFile::copy(m_Filename,baseFilename());
    }

    m_Upload->deleteLater();
    m_Upload = 0;

    emit cityUpdated();
}


Region* City::region()
{
    return (Region*)parent();
}

QString City::baseFilename()
{
    return region()->dataDirectory().absoluteFilePath(m_Guid);
}

QString City::currentFilename()
{
    return m_Filename;
}


QString City::thumbnailUrl()
{
   // return "qrc:///resources/thumbnail.png";


    stringstream s;
    if(m_Thumbnail.isNull())
    {
        s << url().toStdString() << "/" << "thumbnail.png";
    }
    else
    {
        s << "image://" << region()->name().toStdString() << "/" << m_Guid.toStdString();
    }

    return QString::fromStdString(s.str());
}

QPoint City::position()
{
    return m_Position;
}

QPoint City::centerPoint()
{
    return QPoint(m_Position.x()+ m_Size/2, m_Position.y()+ m_Size/2);
}

int City::size()
{
    return m_Size;
}


QPoint City::bottomRightPosition()
{
    return QPoint(m_Position.x()  +m_Size,m_Position.y() + m_Size);
}

QPoint City::bottomLeftPosition()
{
    return QPoint(m_Position.x() ,m_Position.y() + m_Size);
}

QImage& City::thumbnail()
{
    return m_Thumbnail;
}

QString City::name()
{
    return m_Name;
}

int City::screenWidth()
{
    return m_Size * 128;
}

QString City::guid()
{
    return m_Guid;
}

QString City::url()
{
    return region()->server()->url() + "region/" + region()->name() + "/" + m_Guid;
}

void City::notifyUpdated()
{

}

bool City::owned()
{
    return owner() == region()->server()->username();
}
