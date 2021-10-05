#include "netblock.h"
#include "ui_netblock.h"
#include "../device-widget/devicewidget.h"
#include "../policy/policy.h"

NetBlock::NetBlock(QWidget *parent)
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

NetBlock::~NetBlock()
{
    delete ui;
}

void NetBlock::receiveMac(const QString mac)
{
    qDebug() << "receive message : " << mac;
    ui->tabWidget->setCurrentIndex(1);
}
