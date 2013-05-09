#include <QtGui/QApplication>

#include "logindialog.h"
#include "regionview.h"

#include "server.h"
#include "regionselectdialog.h"


#include <QDeclarativeView>

#include "gamesession.h"


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);


  //  GameSession test;

 //   test.createProcess();



    Server server("http://127.0.0.1:6888/");

    RegionSelectDialog select(&server);

    if(select.exec() == QDialog::Accepted)
    {
        RegionView w(&server);
        select.selectedRegion()->prepareForPlay();
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

    return a.exec();
}
