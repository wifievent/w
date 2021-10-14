#include "policy.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    policy w;
    w.show();
    return a.exec();
}
