#ifndef CITY_H
#define CITY_H

#include <QImage>
#include <QObject>
#include <QPixmap>

#include "json/json.h"

class RegionViewFile;
class Region;

class QNetworkReply;

class City : public QObject
{
    Q_OBJECT

public:

    explicit City(Region*, Json::Value&);
    explicit City(Region *region,const QString& path);
    ~City();

    //City now resides in this file
    void updateFromFile(const QString& path);

    //Parsed Json
    void updateFromJson(const Json::Value&);

    void updatedBaseMd5();


    //Get the guid of a save game file
    static QString extractGuid(QString path);

    //Save game file, of the current live server version
    QString baseFilename();

    //Name of the save game as seen by the game
    QString currentFilename();





    Q_INVOKABLE QImage& thumbnail();

    QPoint position();
    QPoint bottomRightPosition();
    QPoint bottomLeftPosition();

    QPoint centerPoint();

    Region* region();

    Q_PROPERTY(QString thumbnailUrl READ thumbnailUrl NOTIFY cityUpdated)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY cityUpdated)
    Q_PROPERTY(float transferPercent READ transferPercent NOTIFY cityUpdated)
    Q_PROPERTY(QString guid READ guid NOTIFY cityUpdated)
    Q_PROPERTY(QString name READ name NOTIFY cityUpdated)
    Q_PROPERTY(QString owner READ owner NOTIFY cityUpdated)    
    Q_PROPERTY(QString claimed READ claimed NOTIFY cityUpdated)
    Q_PROPERTY(QString owned READ owned NOTIFY cityUpdated)


    Q_INVOKABLE QString filename() { return m_Filename; }
    Q_INVOKABLE QString url();
    Q_INVOKABLE QString name();
    Q_INVOKABLE QString owner() { return m_Owner; }
    Q_INVOKABLE bool claimed() { return m_Owner != ""; }
    Q_INVOKABLE bool owned();

    Q_INVOKABLE QString guid();
    Q_INVOKABLE QString thumbnailUrl();

    Q_INVOKABLE int screenWidth();
    int size();

    bool isLoading();

    void notifyUpdated();

    float transferPercent(){ return m_TransferPercen; }

    //Check that the file is there and if not start a download
    void prepareForPlay();



signals:
    void cityUpdated();
public slots:



private slots:
    void uploadCompleted();
    void downloadCompleted();
    void transferProgress(qint64,qint64);
private:

    //Pick a new filename for the city
    void determineNewFilename();

    void startDownload();
    void startUpload(QString oldHash, QString newHash);


    bool m_Modified;

    QNetworkReply* m_Download;
    QNetworkReply* m_Upload;

    QImage m_Thumbnail;

    QString m_HeadMd5; //City checksum of HEAD server version
    QString m_LiveMd5; //City checksum of current version

    QString m_Name;
    QString m_Guid;
    QString m_Owner;

    QString m_Filename;

    QPoint m_Position;
    int m_Size;

    float m_TransferPercen;
};

#endif // CITY_H
