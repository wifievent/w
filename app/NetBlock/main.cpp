#include "netblock_ui.h"
#include "core.h"
#include "db_check.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    int db_check = dbCheck();

    Core core;
    core.start();
    QApplication a(argc, argv);
    NetBlock_UI w;
    w.show();
    a.exec();
    core.stop();
    return 0;
}
