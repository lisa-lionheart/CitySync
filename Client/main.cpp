#include <QtGui/QApplication>

#include "logindialog.h"
#include "regionview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    LoginDialog login;

    int result = login.exec();

    if(result == QDialog::Accepted)
    {
        RegionView w;
        w.show();
        return a.exec();
    }

    return 0;
}
