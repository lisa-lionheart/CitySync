#ifndef REGION_H
#define REGION_H

#include <QObject>
#include <QList>


class City;

enum SyncStatus
{
    Ready,
    Downloading,
    Uploading
};

class Region : public QObject
{
    Q_OBJECT

public:
    Region(QString path);


    const QList<City*>& cities() {
        return m_Cities;
    }

signals:
        //
    void regionUpdated();


private:
    QList<City*> m_Cities;

};

#endif // REGION_H
