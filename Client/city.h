#ifndef CITY_H
#define CITY_H

#include <QImage>
#include <QObject>
#include <QPixmap>

#include "json/json.h"

class RegionViewFile;
class Region;

class City : public QObject
{
    Q_OBJECT


    Q_PROPERTY(QString thumbnailUrl READ thumbnailUrl NOTIFY cityUpdated)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY cityUpdated)
public:
    explicit City(Region*, Json::Value&);
    ~City();
    
    void loadSave(const QString& path);

    Q_INVOKABLE QImage& thumbnail();

    QPoint position();
    QPoint bottomRightPosition();
    QPoint bottomLeftPosition();

    QPoint centerPoint();

    Region* region();

    Q_INVOKABLE QString thumbnailUrl();

    Q_INVOKABLE int screenWidth();
    int size();

    bool isLoading();

    Q_INVOKABLE QString name();

    Q_INVOKABLE QString guid();

signals:
    void cityUpdated();
public slots:
    

private:
    QImage m_Thumbnail;
    QString m_Name;
    QString m_Guid;
    QString m_Owner;

    QPoint m_Position;
    int m_Size;

};

#endif // CITY_H
