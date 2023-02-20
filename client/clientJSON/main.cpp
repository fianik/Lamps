#include "clientjson.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientJSON w;
    w.show();

    return a.exec();
}
