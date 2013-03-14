#include "cityview.h"

#include "city.h"

CityView::CityView(QWidget *parent) :
    QLabel(parent), m_Thumbnail(parent), m_Name(this)
{


}

void CityView::setCity(City* city)
{
    m_City = city;


    setPixmap(m_City->thumbnail());
    resize(m_City->thumbnail().size());

    m_Name.setText(m_City->name());
}
