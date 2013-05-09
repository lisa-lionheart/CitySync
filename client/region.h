#ifndef REGION_H
#define REGION_H

#include <QMap>
#include <QList>
#include <QDir>

#include <QDeclarativeImageProvider>
#include <json/json.h>

class Server;
class City;
class QNetworkReply;

enum SyncStatus
{
    Ready,
    Downloading,
    Uploading
};

class Region : public QObject, public QDeclarativeImageProvider
{
    Q_OBJECT

public:
    Region(Server*,Json::Value& data);
    ~Region();
    Server* server() { return m_Server; }

    Q_INVOKABLE QString name() { return m_Name; }

    const QMap<QString,City*>& cities();

    //id is /guid/thumbnail.png
    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);


    QDir dataDirectory(); //User data directory for this region
    QDir saveDirectory(); //Location of sc4 files

    // Make sure the region is ready to be played
    void prepareForPlay();

signals:
        //
    void regionUpdated();

private slots:
    void handleGotConfigBmp();
    void handleGotCities();

private:

   void readSaveGames();

   QString m_Name;
   QMap<QString,City*> m_Cities;

   QNetworkReply* m_GetCitiesOp;
   QNetworkReply* m_GetConfigBmpOp;
   QDir m_DataDir;
   Server* m_Server;
};

#endif // REGION_H
