#include "regionview.h"
#include "ui_regionview.h"

RegionView::RegionView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegionView)
{
    ui->setupUi(this);
}

RegionView::~RegionView()
{
    delete ui;
}
