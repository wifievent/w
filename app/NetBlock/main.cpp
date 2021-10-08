#include "netblock_ui.h"
#include "db_check.h"
#include "core/core.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    dbCheck();
    Core core;
    core.start();
    QApplication a(argc, argv);
    NetBlock_UI w;
    w.show();
    a.exec();
    core.stop();
    return 0;
}
