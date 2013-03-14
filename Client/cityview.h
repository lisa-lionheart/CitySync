#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QWidget>
#include <QLabel>
#include "city.h"

class CityView : public QLabel
{
    Q_OBJECT
public:
    explicit CityView(QWidget *parent = 0);
    
    void setCity(City*);
signals:
    
public slots:
    
private:
    City* m_City;

    QLabel m_Thumbnail;
    QLabel m_Name;
};

#endif // CITYVIEW_H
