#include "netblock_ui.h"
#include "ui_netblock_ui.h"
#include "device/devicewidget.h"
#include "policy/policy.h"

NetBlock_UI::NetBlock_UI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NetBlock)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowMaximized);

    DeviceWidget *device_widget = new DeviceWidget(this);
    Policy *m_policy = new Policy();

    ui->tabWidget->addTab(device_widget, "Device");
    ui->tabWidget->addTab(m_policy, "Policy");
}

NetBlock_UI::~NetBlock_UI()
{
    delete ui;
}

void NetBlock_UI::closeEvent(QCloseEvent *event)
{
    QSettings settings("WifiEvent", "NetBlock");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("maximized", isMaximized() );
        if ( !isMaximized() ) {
            settings.setValue( "pos", pos() );
            settings.setValue( "size", size() );
        }

    QMainWindow::closeEvent(event);
}

void NetBlock_UI::readSettings()
{
    QSettings settings("WifiEvent", "NetBlock");
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("windowState", saveState()).toByteArray());
    move(settings.value("pos", pos() ).toPoint());
        resize(settings.value("size", size() ).toSize());
        if (settings.value("maximized", isMaximized() ).toBool() )
            showMaximized();
}

void NetBlock_UI::receiveMac(const QString mac)
{
    qDebug() << "receive message : " << mac;
    ui->tabWidget->setCurrentIndex(1);
}
