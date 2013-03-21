#include "region.h"
#include "city.h"
#include "sc4savegame.h"
#include "server.h"
#include <QDir>
#include <QSettings>

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
QString dataDir()
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

}

Region::~Region()
{
    //Delete all the cites
    for(QMap<QString,City*>::iterator itr = m_Cities.begin(); itr != m_Cities.end(); itr++ )
    {
        delete itr.value();
    }
}


const QMap<QString,City*>& Region::cities()
{
    if(m_Cities.size() == 0 && m_GetCitiesOp == NULL)
    {
        QNetworkRequest getCitiesRequest(server()->url() + "region/" + name() );
        server()->applyAuth(getCitiesRequest);

        m_GetCitiesOp = server()->get(getCitiesRequest);
        connect(m_GetCitiesOp,SIGNAL(finished()),SLOT(handleGotCities()));
    }

    return m_Cities;
}

void Region::handleGotCities()
{
    QByteArray data = m_GetCitiesOp->readAll();

    Json::Value root;
    bool parsingSuccessful =  Json::Reader().parse( std::string(data.data(), data.length()) , root );
    if ( !parsingSuccessful )
    {
        return;
    }

    for(int i=0; i < root.size(); i ++)
    {
        City* city = new City(this, root[i]);
        m_Cities[city->guid()] = city;
    }

    m_GetCitiesOp->deleteLater();
    m_GetCitiesOp = 0;
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

QImage Region::requestImage(const QString &id, QSize *size, const QSize& requestedSize)
{
    City* city = m_Cities[id];

    if(city)
        return city->thumbnail();

    return QImage();
}


