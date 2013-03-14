#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QMainWindow>
#include <Region.h>

#include "region.h"



namespace Ui {
class RegionView;
}

class RegionView : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit RegionView();
    ~RegionView();
    
    void setRegion(Region*);
private:
    Ui::RegionView *ui;
};

#endif // REGIONVIEW_H
