#include <QApplication>
#include "mainwindow.h"
#include "netreceiver.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Leadontec");
    app.setApplicationName("LogReceiver");
    app.setOrganizationDomain("Leadon Ltd.");

    MainWindow w;
    w.show();
    return app.exec();
}

