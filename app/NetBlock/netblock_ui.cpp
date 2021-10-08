#include "netblock_ui.h"
#include "ui_netblock.h"
#include "devicewidget.h"
#include "policy.h"

NetBlock_UI::NetBlock_UI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NetBlock_UI)
{
    ui->setupUi(this);

    DeviceWidget *device_widget = new DeviceWidget();
    policy *m_policy = new policy();

    ui->tabWidget->addTab(device_widget, "Device");
    ui->tabWidget->addTab(m_policy, "Policy");
}

NetBlock_UI::~NetBlock_UI()
{
    delete ui;
}

