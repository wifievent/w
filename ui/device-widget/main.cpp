#include <QApplication>
#include "devicewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeviceWidget w;
    w.show();

    return a.exec();
}
