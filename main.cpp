#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("BrickHouseDevelopers");
    a.setOrganizationDomain("Earth");
    a.setApplicationName("journalQT");

    MainWindow w;
    w.show();

    return a.exec();
}
