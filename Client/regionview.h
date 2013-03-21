#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QMainWindow>
#include <Region.h>

#include "region.h"
#include "server.h"

#include <QDeclarativeEngine>

class Region;
namespace Ui {
class RegionView;
}

class RegionView : public QMainWindow
{
    Q_OBJECT
    


public:
    explicit RegionView(Server*);
    ~RegionView();
    
    void setRegion(Region*);
public slots:
    void loadView();
    void regionLoaded();

private:
    Ui::RegionView *ui;

    QDeclarativeEngine m_Engine;
    QDeclarativeComponent* m_Tile;
    Region* m_Region;
    Server* m_Server;
};

#endif // REGIONVIEW_H
