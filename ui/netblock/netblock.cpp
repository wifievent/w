#include "netblock.h"
#include "ui_netblock.h"
#include "../device-widget/devicewidget.h"
#include "../policy/policy.h"

NetBlock::NetBlock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NetBlock)
{
    ui->setupUi(this);

    DeviceWidget *device_widget = new DeviceWidget();
    policy *m_policy = new policy();

    ui->tabWidget->addTab(device_widget, "Device");
    ui->tabWidget->addTab(m_policy, "Policy");
}

NetBlock::~NetBlock()
{
    delete ui;
}

