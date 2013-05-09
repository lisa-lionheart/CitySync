#ifndef REGIONSELECTDIALOG_H
#define REGIONSELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>


class Server;
class Region;

namespace Ui {
class RegionSelectDialog;
}

class RegionSelectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RegionSelectDialog(Server *server);
    ~RegionSelectDialog();

    Region* selectedRegion(){ return m_SelectedRegion; }

public slots:
    void gotRegions();

private slots:
    void on_regionList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_regionList_itemSelectionChanged();

private:
    Ui::RegionSelectDialog *ui;

    Server* m_Server;
    Region* m_SelectedRegion;
};

#endif // REGIONSELECTDIALOG_H
