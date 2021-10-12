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
    QIcon icon(":/image/logo/logo.ico");
    a.setWindowIcon(icon);
    NetBlock_UI w;
    w.readSettings();
    w.show();
    a.exec();
    core.stop();
    return 0;
}
