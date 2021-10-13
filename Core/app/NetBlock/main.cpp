#include "netblock_ui.h"
#include "db_check.h"
#include "core/core.h"

#include <QApplication>

const char* version()
{
    return
#ifdef _DEBUG
#include "../../version.txt"
    " Debug Build(" __DATE__ " " __TIME__ ")";
#else // RELEASE
#include "../../version.txt"
    " Release Build(" __DATE__ " " __TIME__ ")";
#endif // _DEBUG
}

int main(int argc, char *argv[])
{
    GTRACE("NetBlock Started %s",version());
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
    GTRACE("NetBlock Finished");
    return 0;
}
