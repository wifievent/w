#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setDevInfo();
    setTableView();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::setDevInfo()
{
    dInfo dummy1;
    dummy1.mac = "AA:BB:CC:DD:EE:FF";
    dummy1.ip = "192.168.111.111";
    dummy1.name = "iphone12";

    dInfo dummy2;
    dummy2.mac = "AA:BB:CC:EE:FF:DD";
    dummy2.ip = "192.168.111.222";
    dummy2.name = "ipad air4";

    devices.push_back(dummy1);
    devices.push_back(dummy2);
}

void Widget::setTableView()
{
    int rowCount = devices.size();
    ui->devTable->setRowCount(rowCount);
    ui->devTable->setColumnCount(2);

    for (int i = 0; i < rowCount; i ++) {
        ui->devTable->setItem(i, 0, new QTableWidgetItem(devices[i].ip));
        ui->devTable->setItem(i, 1, new QTableWidgetItem(devices[i].name));

    }
    // 테이블 수정 불가
    ui->devTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 테이블 크기 조정
    ui->devTable->resizeColumnsToContents();
    ui->devTable->horizontalHeader()->setStretchLastSection(true);
}

void Widget::on_devTable_cellClicked(int row, int column)
{
    ui->devInfo->clear();
    ui->devInfo->addItem("OUI");
    ui->devInfo->addItem("MAC\t" + devices[row].mac);
    ui->devInfo->addItem("IP\t" + devices[row].ip);
    ui->devInfo->addItem("Name\t" + devices[row].name);
}

