#include "region.h"
#include "city.h"
#include "sc4savegame.h"
#include "server.h"
#include <QDir>
#include <QSettings>
#include <Qdebug>

/*
Region::Region(QString path) :
    QDeclarativeImageProvider(Image)
{

    QDir regionDirectory(path);

    QStringList filter;
    filter.append("*.sc4");
    QStringList cityFiles = regionDirectory.entryList(filter);

    for(int i=0; i < cityFiles.size(); i++)
    {
        City* city = new City(this);
        city->loadSave(path + cityFiles[i]);
        m_Cities[city->guid()] = city;
    }
}
*/
inline QString dataDir()
{
    QSettings ini(QSettings::IniFormat, QSettings::UserScope,
                  "CitySync","Client");
    return QFileInfo(ini.fileName()).absolutePath() + "/";
}

Region::Region(Server* _server, Json::Value& data ) :
    m_Server(_server),
    QDeclarativeImageProvider(Image)
{
    m_GetConfigBmpOp = NULL;
    m_GetCitiesOp = NULL;
    m_Name = QString::fromStdString( data["name"].asString() );


    QString serverId = server()->url();
    serverId = serverId.replace("/","").replace(":","");
    m_DataDir = QDir(dataDir() + serverId + "/" + m_Name);

    if(m_DataDir.mkpath("Regions/" + m_Name))
    {
        // Get the config.bmp

        QNetworkRequest bmpRequest(server()->url() + "region/" + name() + "/config.bmp" );


        m_GetConfigBmpOp = server()->get(bmpRequest);
        connect(m_GetConfigBmpOp, SIGNAL(finished()),SLOT(handleGotConfigBmp()));
    }

    qDebug() << "Created Region: " << m_Name;
}



Region::~Region()
{
    //Delete all the cites
    for(QMap<QString,City*>::iterator itr = m_Cities.begin(); itr != m_Cities.end(); itr++ )
    {
        delete itr.value();
    }
}


void Region::prepareForPlay()
{
    QMap<QString,City*>::iterator itr = m_Cities.begin();
    while(itr != m_Cities.end())
    {
        itr.value()->prepareForPlay();
        itr++;
    }
}

const QMap<QString,City*>& Region::cities()
{
    if(m_Cities.size() == 0 && m_GetCitiesOp == NULL)
    {

        QNetworkRequest getCitiesRequest(server()->url() + "region/" + name());

        m_GetCitiesOp = server()->get(getCitiesRequest);
        connect(m_GetCitiesOp,SIGNAL(finished()),SLOT(handleGotCities()));
    }

    return m_Cities;
}


void Region::readSaveGames()
{
    QStringList files = saveDirectory().entryList();

    for(int i = 0; i < files.length(); i++)
    {
        QString file = files[i];

        QFileInfo info(saveDirectory().absoluteFilePath(file));
        if(!info.exists() || info.size() == 0)
        {
            qDebug() << "File does not exist";
            continue;
        }

        if(file.endsWith(".sc4"))
        {
            try
            {
                City* city = new City(this,saveDirectory().absoluteFilePath(file));

                if(city->guid().length() != 0)
                 m_Cities[city->guid()] = city;

            }
            catch(std::exception e)
            {
                qDebug() << "Exception loading city from file " << file;
            }
        }
    }

}


void Region::handleGotCities()
{
    readSaveGames();

    QByteArray data = m_GetCitiesOp->readAll();

    Json::Value root;
    bool parsingSuccessful =  Json::Reader().parse( std::string(data.data(), data.length()) , root );
    if ( !parsingSuccessful )
    {
        return;
    }

    Json::Value cityData;
    foreach(cityData,root["cities"])
    {
        QString guid = QString::fromStdString( cityData["guid"].asString() );

        if(m_Cities[guid] == NULL)
        {
            City* city = new City(this, cityData);
            m_Cities[guid] = city;
        }
        else
        {
            m_Cities[guid]->updateFromJson(cityData);
        }
    }

    m_GetCitiesOp->deleteLater();
    m_GetCitiesOp = 0;

    prepareForPlay();
    emit regionUpdated();
}

void Region::handleGotConfigBmp()
{
    QFile configBmp(saveDirectory().absoluteFilePath("config.bmp"));
    configBmp.open(QFile::WriteOnly);
    configBmp.write(m_GetConfigBmpOp->readAll());
    configBmp.close();

    m_GetConfigBmpOp->deleteLater();
    m_GetConfigBmpOp = NULL;
}


QDir Region::saveDirectory()
{
    return QDir(m_DataDir.path() + "/Regions/" + m_Name + "/");
}

QDir Region::dataDirectory()
{
    return m_DataDir;
}

QImage Region::requestImage(const QString &id, QSize *size, const QSize& requestedSize)
{
    City* city = m_Cities[id];

    if(city)
        return city->thumbnail();

    return QImage();
}


