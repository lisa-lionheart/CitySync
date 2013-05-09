#include "regionselectdialog.h"
#include "ui_regionselectdialog.h"

#include "region.h"
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

    QMovie* loadingMovie = new QMovie( ":/resources/ajax-loader.gif" , QByteArray(), this);
    ui->ajaxLoader->setMovie(loadingMovie);
    loadingMovie->start();
}

RegionSelectDialog::~RegionSelectDialog()
{
    delete ui;
}


void RegionSelectDialog::gotRegions()
{
    QList<QString> regions = m_Server->regions().keys();
    ui->regionList->insertItems(0,regions);

   // ui->regionList->setSelection( );

    ui->ajaxLoader->hide();

    ui->buttonBox->setEnabled(true);
    ui->regionList->setEnabled(true);
}

void RegionSelectDialog::on_regionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    m_SelectedRegion = m_Server->regions()[current->text()];

    QNetworkRequest request(QUrl(m_Server->url() + "/region/" + m_SelectedRegion->name() + "/html"));
    ui->webView->load(request);
}

void RegionSelectDialog::on_regionList_itemSelectionChanged()
{

}
