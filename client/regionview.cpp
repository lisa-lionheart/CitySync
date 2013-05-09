
#include <QDeclarativeItem>
#include <QGraphicsView>

#include <QScrollArea>

#include "regionview.h"
#include "ui_regionview.h"

#include "cityview.h"
#include "math.h"
#include "gamesession.h"



RegionView::RegionView(Server* server) :
    QMainWindow(0),
    ui(new Ui::RegionView)
{
    m_Server = server;

    m_Tile = new QDeclarativeComponent(&m_Engine,"resources/tile.qml");

    if(m_Tile->errors().length() != 0)
        qDebug() << m_Tile->errorString();


    ui->setupUi(this);

    /*
    setStyleSheet("background:transparent;");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    */
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
        connect(m_Region,SIGNAL(regionUpdated()),SLOT(loadView()));
    }else{
        loadView();
    }
}

void RegionView::loadView()
{
    QGraphicsScene* scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

    scene->setBackgroundBrush(QBrush(QColor(128,128,0)));

    City* c;
    foreach(c,m_Region->cities())
    {
        QDeclarativeContext* ctx = new QDeclarativeContext(&m_Engine,scene);
        ctx->setContextProperty("region",m_Region);
        ctx->setContextProperty("city",c);
        ctx->setContextProperty("view",this);

        QDeclarativeItem* cityView = (QDeclarativeItem*)m_Tile->create(ctx);


        int x = project(c->bottomLeftPosition()).x();
        int y = project(c->bottomRightPosition()).y() - 800;

        cityView->setProperty("x", x);
        cityView->setProperty("y", y);
        cityView->setProperty("z", project(c->centerPoint()).y() + c->size()*5 );


        scene->addItem( cityView );
    }


}

RegionView::~RegionView()
{
    delete ui;
}

void RegionView::setSelectedGuid(QString guid)
{
    m_SelectedGuid = guid;
}

void RegionView::onGameExited()
{
    ui->launch->setEnabled(true);
}

void RegionView::on_launch_clicked()
{
    GameSession* gameSession = new GameSession(this);

    connect(gameSession,SIGNAL(gameExited()),this,SLOT(onGameExited()));
    ui->launch->setEnabled(false);

    gameSession->playRegion(m_Region);
}
