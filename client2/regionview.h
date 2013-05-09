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
    
    Q_PROPERTY(QString selectedGuid READ selectedGuid)

public:
    explicit RegionView(Server*);
    ~RegionView();
    
    Q_INVOKABLE QString selectedGuid(){ return m_SelectedGuid; }
    Q_INVOKABLE void setSelectedGuid(QString);

    void setRegion(Region*);
public slots:
    void loadView();

private slots:
    void onGameExited();

    void on_launch_clicked();

private:
    Ui::RegionView *ui;

    QString m_SelectedGuid;

    QDeclarativeEngine m_Engine;
    QDeclarativeComponent* m_Tile;
    Region* m_Region;
    Server* m_Server;
};

#endif // REGIONVIEW_H
