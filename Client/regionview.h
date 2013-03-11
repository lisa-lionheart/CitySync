#ifndef REGIONVIEW_H
#define REGIONVIEW_H

#include <QMainWindow>

namespace Ui {
class RegionView;
}

class RegionView : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit RegionView(QWidget *parent = 0);
    ~RegionView();
    
private:
    Ui::RegionView *ui;
};

#endif // REGIONVIEW_H
