#include <QtGui/QApplication>

#include "logindialog.h"
#include "regionview.h"

#include "server.h"
#include "regionselectdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server server("http://192.168.0.6:8888/");

    RegionSelectDialog select(&server);

    if(select.exec() == QDialog::Accepted)
    {
        RegionView w(&server);
        w.setRegion(select.selectedRegion());
        w.show();
        return a.exec();
    }

/*
    LoginDialog login;

    int result = login.exec();

    if(result == QDialog::Accepted)
    {*/
        //Region* region = new Region("C:\\Users\\Lisa\\CitySync\\test\\Regions\\Steamboat Springs\\");


    //}

    //return 0;
}
