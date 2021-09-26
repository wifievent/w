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

// dummy data for test
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
    QStringList colHeader;
    colHeader << "IP" << "Name" ;
    ui->devTable->setHorizontalHeaderLabels(colHeader);
    ui->devTable->setRowCount(devices.size());
    ui->devTable->setColumnCount(2);

    for (int i = 0; i < (int)devices.size(); i ++) {
        ui->devTable->setItem(i, 0, new QTableWidgetItem(devices[i].ip));
        ui->devTable->setItem(i, 1, new QTableWidgetItem(devices[i].name));

    }
    // 테이블 수정 불가
    ui->devTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 테이블 크기 조정
    ui->devTable->horizontalHeader()->setStretchLastSection(true);
}

// dev Table 에서 cell 클릭 시 리스트에 해당 device info view
void Widget::on_devTable_cellClicked(int row, int column)
{
    dinfo.mac = devices[row].mac;
    dinfo.ip = devices[row].ip;
    dinfo.name = devices[row].name;
    dinfo.vectorID = row;

    ui->devInfo->clear();
    ui->devInfo->addItem("OUI\t" + dinfo.oui);
    ui->devInfo->addItem("MAC\t" + dinfo.mac);
    ui->devInfo->addItem("IP\t" + dinfo.ip);
    ui->devInfo->addItem("Name\t" + dinfo.name);
}


void Widget::on_researchBtn_clicked()
{
    // clear
    ui->devTable->clear();
    ui->devInfo->clear();
    // 재탐색 실행
    // todo

    // 테이블 view
    setTableView();
}


void Widget::on_DeleteBtn_clicked()
{
    devices.erase(devices.begin() + dinfo.vectorID);

    ui->devInfo->clear();
    ui->devTable->clear();
    setTableView();
}

