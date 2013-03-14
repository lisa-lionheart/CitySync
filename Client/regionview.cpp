#include "regionview.h"
#include "ui_regionview.h"

#include "cityview.h"
#include <QScrollArea>

#include "math.h"

RegionView::RegionView() :
    QMainWindow(0),
    ui(new Ui::RegionView)
{



    ui->setupUi(this);


}


bool orderOnY(City* a, City* b)
{
    return a->tilePosition().y() < b->tilePosition().y();
}


float distToOrigin(QPoint p)
{
    return sqrt( (p.x() * p.x()) + (p.y() * p.y()) );
}

bool orderDist(City* a, City* b)
{
    return distToOrigin(a->position()) > distToOrigin(b->tilePosition());
}



void RegionView::setRegion(Region* region)
{


    QList<City*> cities = region->cities();

    qSort(cities.begin(),cities.end(),orderOnY);
    QLabel* container = new QLabel;

    container->resize(2100,1000);

    for(int i=0; i < cities.length(); i++)
    {

        CityView* cityView = new CityView(container);
        cityView->setCity(cities[i]);

        QPoint basePos = cities[i]->tilePosition();

        int offsetX = ((cities[i]->thumbnail().width() * 184) /260);
        int offsetY = cities[i]->thumbnail().height();
        cityView->move(basePos.x() - offsetX + 600, basePos.y() - offsetY + 30);
    }

    QScrollArea* scroll = new QScrollArea(this);
    setCentralWidget(scroll);

    scroll->setWidget(container);


}

RegionView::~RegionView()
{
    delete ui;
}
