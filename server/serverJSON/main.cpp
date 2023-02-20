#include "serverjson.h"
#include <QApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Server Srvr;
    ServerJSON WndwSrvr;

    WndwSrvr.show();

    QObject::connect(&Srvr, SIGNAL(resAvailable(QString)),&WndwSrvr, SLOT(connectionState(QString)));
    QObject::connect(&Srvr, SIGNAL(itemUpdate()),&WndwSrvr, SLOT(updateItem()));

    return app.exec();
}
