#include "netblock.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetBlock w;
    w.show();
    return a.exec();
}
