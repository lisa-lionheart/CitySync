#include "regionview.h"
#include "ui_regionview.h"

#include "cityview.h"
#include <QScrollArea>

#include "math.h"

#include <QDeclarativeItem>
#include <QGraphicsView>

RegionView::RegionView(Server* server) :
    QMainWindow(0),
    ui(new Ui::RegionView)
{
    m_Server = server;

    ui->setupUi(this);

}


QPoint project(QPoint region)
{
    int x = (region.x() * 90) - (region.y() * 37);
    int y = (region.y() * 45) + (region.x() * 19);

    return QPoint(x,y);
}

void RegionView::setRegion(Region* region)
{
    m_Region = region;

    m_Engine.addImageProvider(region->name(), region);
    if(m_Region->cities().count() == 0)
    {
        connect(m_Region,SIGNAL(regionUpdated()),SLOT(regionLoaded()));
    }
}


void RegionView::regionLoaded()
{


    m_Tile = new QDeclarativeComponent(&m_Engine, QUrl("resources/tile.qml"));


    if(!m_Tile->isReady())
        QObject::connect(m_Tile, SIGNAL(statusChanged(QDeclarativeComponent::Status)),
                          this, SLOT(loadView()));
    else
        loadView();

}

void RegionView::loadView()
{

    QList<City*> cities = m_Region->cities().values();

    QGraphicsScene* scene = new QGraphicsScene;
    QGraphicsView* container = new QGraphicsView(scene);

    container->setBackgroundBrush(QBrush(QColor(128,0,255)));

    container->resize(2100,1000);

    for(int i=0; i < cities.length(); i++)
    {
        City* c = cities[i];
/*
        CityViewController* cityViewController = new CityViewController(&m_Engine);
        cityViewController->setCity(cities[i]);
*/

        QDeclarativeContext* ctx = new QDeclarativeContext(&m_Engine,container);
        ctx->setContextProperty("region",m_Region);
        ctx->setContextProperty("city",c);

        QDeclarativeItem* cityView = (QDeclarativeItem*)m_Tile->create(ctx);

        int x = project(c->bottomLeftPosition()).x();
        int y = project(c->bottomRightPosition()).y() - 400;

        QPoint pos = c->position();

        cityView->setProperty("x", x);
        cityView->setProperty("y", y);
        cityView->setProperty("z", project(c->centerPoint()).y() + c->size()*5 );

        container->scene()->addItem( cityView );
    }

/*
    for(int i=0; i <= 16; i++ )
    {
        scene->addLine(QLine(project(QPoint(i,0)),project(QPoint(i,16))));
        scene->addLine(QLine(project(QPoint(0,i)),project(QPoint(16,i))));
    }
*/

    setCentralWidget(container);


}

RegionView::~RegionView()
{
    delete ui;
}
