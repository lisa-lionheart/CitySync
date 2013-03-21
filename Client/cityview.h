#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QWidget>
#include <QLabel>
#include "city.h"

#include <QDeclarativeContext>

class CityViewController : public QDeclarativeContext
{
    Q_OBJECT
public:
    explicit CityViewController(QDeclarativeEngine*);

    void setCity(City*);
signals:
    
public slots:

    
private:
    City* m_City;
};

#endif // CITYVIEW_H
