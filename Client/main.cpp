#include <QtGui/QApplication>

#include "logindialog.h"
#include "regionview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*
    LoginDialog login;

    int result = login.exec();

    if(result == QDialog::Accepted)
    {*/
        Region* region = new Region("C:\\Users\\Lisa\\CitySync\\test\\Regions\\Tamriel Small\\");

        RegionView w;
        w.setRegion(region);
        w.show();
        return a.exec();
    //}

    //return 0;
}
