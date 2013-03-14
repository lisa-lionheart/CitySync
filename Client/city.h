#ifndef CITY_H
#define CITY_H

#include <QImage>
#include <QObject>
#include <QPixmap>

class RegionViewFile;

class City : public QObject
{
    Q_OBJECT
public:
    explicit City(QObject*);
    
    void loadSave(const QString& path);

    QPixmap& thumbnail();

    QPoint tilePosition();
    QPoint position();

    QString name();

signals:
    
public slots:
    

private:
    QPixmap m_Thumbnail;
    RegionViewFile* m_RegionData;
};

#endif // CITY_H
