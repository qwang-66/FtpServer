#include <QCoreApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Widget w;
    w.start();
    return a.exec();
}
