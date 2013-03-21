#include "regionselectdialog.h"
#include "ui_regionselectdialog.h"

#include "server.h"
#include <QMovie>

RegionSelectDialog::RegionSelectDialog(Server *server) :
    ui(new Ui::RegionSelectDialog)
{
    m_Server = server;
    m_SelectedRegion = 0;

    m_Server->fetchRegions();
    connect(m_Server,SIGNAL(regionsUpdated()),SLOT(gotRegions()));

    ui->setupUi(this);
    ui->ajaxLoader->setMovie(new QMovie( "qrc:///resources/ajax-loader.gif" ));
}

RegionSelectDialog::~RegionSelectDialog()
{
    delete ui;
}


void RegionSelectDialog::gotRegions()
{
    QList<QString> regions = m_Server->regions().keys();
    ui->regionList->insertItems(0,regions);
}

void RegionSelectDialog::on_regionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    m_SelectedRegion = m_Server->regions()[current->text()];
}
