#include "netblock_ui.h"
#include "ui_netblock_ui.h"
#include "device/devicewidget.h"
#include "policy/policy.h"

NetBlock_UI::NetBlock_UI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NetBlock)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    DeviceWidget *device_widget = new DeviceWidget(this);
    policy *m_policy = new policy();

    ui->tabWidget->addTab(device_widget, "Device");
    ui->tabWidget->addTab(m_policy, "Policy");
}

NetBlock_UI::~NetBlock_UI()
{
    delete ui;
}

void NetBlock_UI::receiveMac(const QString mac)
{
    qDebug() << "receive message : " << mac;
    ui->tabWidget->setCurrentIndex(1);
}
