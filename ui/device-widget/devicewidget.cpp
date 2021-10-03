#include "devicewidget.h"
#include "ui_devicewidget.h"

DeviceWidget::DeviceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    db_connect = new DB_Connect("test.db");
    setDummyDB();
    setDevInfo();
    setDevTableWidget();
    initDevListWidget();

    connect(
        ui->devTable,
        SIGNAL(itemSelectionChanged()),
        this,
        SLOT(slotSelectionChanged())
    );
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

// Disable button on empty space click
void DeviceWidget::slotSelectionChanged()
{
    initDevListWidget();
}

void DeviceWidget::setDevState()
{
    // true -> active
    while(true) {
        sleep(10);
        for(int i = 0; i < (int)devices.size(); i++) {
            QString qmac_ = devices[i].mac;
            std::string mac_ = qmac_.toStdString();
            qDebug() << "Current MAC : " << qmac_;
            // devices[i].is_active = FullScan::isConnect(mac_);
        }
    }
}

void DeviceWidget::setDummyDB()
{
    // init for test
    db_connect->send_query("DROP TABLE IF EXISTS host");
    db_connect->send_query("DROP TABLE IF EXISTS time");
    db_connect->send_query("DROP TABLE IF EXISTS policy");

    db_connect->send_query("CREATE TABLE host (\
    host_id	INTEGER		NOT NULL PRIMARY KEY AUTOINCREMENT,\
    mac		CHAR(17)	NOT NULL,\
    last_ip	VARCHAR(15)	NULL,\
    name	VARCHAR(30)	NULL)");

    db_connect->send_query("CREATE TABLE time (\
    time_id			INTEGER	NOT NULL PRIMARY KEY AUTOINCREMENT,\
    start_time		CHAR(4)	NOT NULL,\
    end_time		CHAR(4)	NOT NULL,\
    day_of_the_week	TINYINT	NOT NULL)");

    db_connect->send_query("CREATE TABLE policy (\
    policy_id	INTEGER	NOT NULL PRIMARY KEY AUTOINCREMENT,\
    host_id		INTEGER	NOT NULL,\
    time_id		INTEGER	NOT NULL)");

    // insert host table data
    db_connect->send_query("INSERT INTO host VALUES(1, '40:70:F5:AA:AA:AA', '192.168.1.101', 'Kim Apple')");
    db_connect->send_query("INSERT INTO host VALUES(2, '70:CE:8C:BB:BB:BB', '192.168.1.102', 'Lee Samsung')");
    db_connect->send_query("INSERT INTO host VALUES(3, '20:DF:B9:CC:CC:CC', '192.168.1.103', 'Park Google')");
    db_connect->send_query("INSERT INTO host VALUES(4, '99:11:11:DD:DD:DD', '192.168.1.104', 'Jegal Kakao')");

    // insert time table data
    db_connect->send_query("INSERT INTO time VALUES(1, '2200', '0830', 1)");
    db_connect->send_query("INSERT INTO time VALUES(2, '2200', '0830', 2)");
    db_connect->send_query("INSERT INTO time VALUES(3, '2200', '0830', 3)");
    db_connect->send_query("INSERT INTO time VALUES(4, '2200', '0830', 4)");
    db_connect->send_query("INSERT INTO time VALUES(5, '2200', '0830', 5)");
    db_connect->send_query("INSERT INTO time VALUES(6, '1200', '1500', 1)");
    db_connect->send_query("INSERT INTO time VALUES(7, '1600', '1800', 2)");
    db_connect->send_query("INSERT INTO time VALUES(8, '1200', '1800', 3)");

    // insert policy table data
    db_connect->send_query("INSERT into policy	VALUES(1, 1, 1)");
    db_connect->send_query("INSERT into policy	VALUES(2, 1, 2)");
    db_connect->send_query("INSERT into policy	VALUES(3, 1, 3)");
    db_connect->send_query("INSERT into policy	VALUES(4, 1, 4)");
    db_connect->send_query("INSERT into policy	VALUES(5, 1, 5)");
    db_connect->send_query("INSERT into policy	VALUES(6, 1, 6)");
    db_connect->send_query("INSERT into policy	VALUES(7, 1, 7)");
    db_connect->send_query("INSERT into policy	VALUES(8, 3, 1)");
    db_connect->send_query("INSERT into policy	VALUES(9, 3, 2)");
    db_connect->send_query("INSERT into policy	VALUES(10, 3, 3)");
    db_connect->send_query("INSERT into policy	VALUES(11, 3, 4)");
    db_connect->send_query("INSERT into policy	VALUES(12, 3, 5)");
    db_connect->send_query("INSERT into policy	VALUES(13, 3, 8)");
}

// Set Device List from DB to Vector
void DeviceWidget::setDevInfo()
{
    std::list<Data_List> dl;
    dl = db_connect->select_query("SELECT * FROM host");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc / 4; ++i) {
            dInfo tmp;
            tmp.host_id = atoi(iter->argv[0 + i * 4]);
            tmp.mac = iter->argv[1 + i * 4];
            tmp.last_ip = iter->argv[2 + i * 4];
            tmp.name = iter->argv[3 + i * 4];
            devices.push_back(tmp);
        }
    }

    Data_List::list_free(dl);
}

// Set the device's state in the table widget
void DeviceWidget::viewDevState()
{
    for (int i = 0; i < (int)devices.size(); i ++) {
        if(devices[i].is_active) {
            ui->devTable->setItem(i, 0, new QTableWidgetItem(""));

            QPushButton *btn = new QPushButton();
            btn->setParent(ui->devTable);
            btn->setStyleSheet("QPushButton { margin: 4px; background-color: blue; width: 20px; border-color: black; border-width: 1px; border-radius: 10px; }");
            ui->devTable->setCellWidget(i, 0, btn);

        }else {
            QPushButton *btn = new QPushButton();
            btn->setParent(ui->devTable);
            btn->setStyleSheet("QPushButton { margin: 4px; background-color: red; width: 20px; border-color: black; border-width: 1px; border-radius: 10px; }");
            ui->devTable->setCellWidget(i, 0, btn);
        }
        ui->devTable->setItem(i, 1, new QTableWidgetItem(devices[i].last_ip));
        ui->devTable->setItem(i, 2, new QTableWidgetItem(devices[i].name));
    }
}

// Set the device list in the table widget
void DeviceWidget::setDevTableWidget()
{
    QStringList colHeader;
    colHeader << "" << "IP" << "Name" ;
    ui->devTable->setColumnCount(3);
    ui->devTable->setHorizontalHeaderLabels(colHeader);
    ui->devTable->setRowCount(devices.size());

    viewDevState();
    // Table can't be modified
    ui->devTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->devTable->setFocusPolicy(Qt::NoFocus);
    ui->devTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->devTable->resizeColumnToContents(0);
    ui->devTable->resizeColumnToContents(1);
    ui->devTable->horizontalHeader()->setStretchLastSection(true);
}

void DeviceWidget::initDevListWidget()
{
    ui->devInfo->clear();
    ui->policyBtn->setEnabled(false);
    ui->deleteBtn->setEnabled(false);
}

void DeviceWidget::activateBtn()
{
    ui->policyBtn->setEnabled(true);
    ui->deleteBtn->setEnabled(true);
}

// Show the deivce info in the list
// When click the cell
void DeviceWidget::on_devTable_cellClicked(int row, int column)
{
    initDevListWidget();
    dinfo.mac = devices[row].mac;
    dinfo.last_ip = devices[row].last_ip;
    dinfo.name = devices[row].name;
    dinfo.vectorID = row;

    ui->devInfo->addItem("OUI\t" + dinfo.oui);
    ui->devInfo->addItem("MAC\t" + dinfo.mac);
    ui->devInfo->addItem("IP\t" + dinfo.last_ip);
    ui->devInfo->addItem("Name\t" + dinfo.name);
    activateBtn();
}

// Clear device info vector
void DeviceWidget::clear_devices()
{
    devices.clear();
}

void DeviceWidget::on_reloadBtn_clicked()
{
    // Clear View & Devices Vector
    ui->devTable->clear();
    clear_devices();
    initDevListWidget();
    setDevInfo();
    setDevTableWidget();
}

void DeviceWidget::on_policyBtn_clicked()
{
    // todo
    // When clicked
    // Go to the Policies tab with the applicable device filter
}


void DeviceWidget::on_deleteBtn_clicked()
{
    int host_id_ = devices[dinfo.vectorID].host_id;
    QString qmac_ = devices[dinfo.vectorID].mac;
    std::string mac_ = qmac_.toStdString();
    devices.erase(devices.begin() + dinfo.vectorID);

    // Delete Data of host table
    // fsmap -> delete req
    // fs.delHost(mac_);
    db_connect->send_query("DELETE FROM host WHERE host_id = " + std::to_string(host_id_));

    ui->devInfo->clear();
    ui->devTable->clear();
    setDevTableWidget();
}

