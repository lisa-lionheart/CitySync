#include "cityview.h"

#include "city.h"

CityViewController::CityViewController(QDeclarativeEngine *parent) :
    QDeclarativeContext(parent)
{


}

void CityViewController::setCity(City* city)
{
    m_City = city;


    //setPixmap(m_City->thumbnail());
    //resize(m_City->thumbnail().size());

    //m_Name.setText(m_City->name());
}
