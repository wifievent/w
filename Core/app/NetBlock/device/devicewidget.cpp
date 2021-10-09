#include "devicewidget.h"
#include "ui_devicewidget.h"
#include "../core/oui.h"
DeviceWidget::DeviceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    setDevInfo();
    setDevTableWidget();
    initDevListWidget();

    timer = new QTimer(this);

    connect(
        ui->devTable,
        SIGNAL(itemSelectionChanged()),
        this,
        SLOT(slotSelectionChanged())
    );

    connect(timer, SIGNAL(timeout()), this, SLOT(updateDevState()));
    timer->start(10000);

    // connect the "clicked" signal to the "buttonClicked" slot
    connect(ui->policyBtn, SIGNAL(clicked()), this, SLOT(on_policyBtn_clicked()));
    // connect the child's "sendMac" signal to the parent's "receiveMac" slot
    connect(this, SIGNAL(sendMac(QString)), parent, SLOT(receiveMac(QString)));
}

DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::updateDevState()
{
    for(int i = 0; i < (int)devices.size(); i++) {
        bool check = fs_instance.isConnect(devices[i].mac.toStdString());

        /*
        if((check = FullScan::isConnect(devices[i].mac.toStdString())) == NULL){
            qDebug() << "[" << i << "] devices.mac check : NULL";
            continue;
        }
        */

        devices[i].is_connect = check;
    }
    viewDevState();
}

// Disable button on empty space click
void DeviceWidget::slotSelectionChanged()
{
    initDevListWidget();
}

// Set Device List from DB to Vector
void DeviceWidget::setDevInfo()
{
    std::list<Data_List> dl;
    dl = db_connect.select_query("SELECT * FROM host");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        dInfo tmp;
        tmp.host_id = stoi(iter->argv[0]);
        tmp.mac = QString::fromStdString(iter->argv[1]);
        tmp.oui = QString(oui_db(WMac(tmp.mac.toStdString())));
        tmp.last_ip = QString::fromStdString(iter->argv[2]);
        tmp.name = QString::fromStdString(iter->argv[3]);
        tmp.oui = QString(oui_db(WMac(tmp.mac.toStdString())));
        // tmp.is_connect = FullScan::isConnect(tmp.mac.toStdString());
        devices.push_back(tmp);
    }

    Data_List::list_free(dl);
}

// Set the device's state in the table widget
void DeviceWidget::viewDevState()
{
    for (int i = 0; i < (int)devices.size(); i ++) {
        if(devices[i].is_connect) {
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
    for(int i = 0; i < (int)devices.size(); i++) {
        ui->devTable->setItem(i, 1, new QTableWidgetItem(devices[i].last_ip));
        ui->devTable->setItem(i, 2, new QTableWidgetItem(devices[i].name));
    }
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

void DeviceWidget::setListWidgetItem(QString str)
{
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *key_label = new QLabel(str + "\t");
    QLabel *val_label = new QLabel();
    if(str == "OUI") {
        val_label->setText(dinfo.oui);
    }else if(str == "MAC") {
        val_label->setText(dinfo.mac);
    }else if(str == "IP") {
        val_label->setText(dinfo.last_ip);
    }
    layout->addWidget(key_label);
    layout->addWidget(val_label);
    layout->addStretch();
    widget->setLayout(layout);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(widget->sizeHint());
    ui->devInfo->addItem(item);
    ui->devInfo->setItemWidget(item, widget);
}

void DeviceWidget::onEditBtnClicked()
{
    std::string name_= lineEdit->text().toStdString();
    qDebug() << "lineedit name : " << QString::fromStdString(name_);
    db_connect.send_query("UPDATE host SET name='" + name_ + "' WHERE host_id=" + std::to_string(dinfo.host_id));
    ui->devTable->clear();
    clear_devices();
    initDevListWidget();
    setDevInfo();
    setDevTableWidget();
}

// Show the deivce info in the list
// When click the cell
void DeviceWidget::on_devTable_cellClicked(int row, int column)
{
    ui->devInfo->clear();
    dinfo.oui = devices[row].oui;
    dinfo.host_id = devices[row].host_id;
    dinfo.mac = devices[row].mac;
    dinfo.last_ip = devices[row].last_ip;
    dinfo.name = devices[row].name;
    dinfo.vectorID = row;

    setListWidgetItem("OUI");
    setListWidgetItem("MAC");
    setListWidgetItem("IP");

    QWidget *n_widget = new QWidget();
    QHBoxLayout *n_layout = new QHBoxLayout();
    QLabel *n_label = new QLabel("Name\t");
    QLineEdit *n_edit = new QLineEdit();
    QPushButton *n_btn = new QPushButton("Edit");
    n_edit->setText(dinfo.name);
    n_layout->addWidget(n_label);
    n_layout->addWidget(n_edit);
    n_layout->addWidget(n_btn);
    n_layout->addStretch();
    n_widget->setLayout(n_layout);

    QListWidgetItem *n_item = new QListWidgetItem();
    n_item->setSizeHint(n_widget->sizeHint());
    ui->devInfo->addItem(n_item);
    ui->devInfo->setItemWidget(n_item, n_widget);

    lineEdit = n_edit;

    connect(n_btn, SIGNAL(clicked()), this, SLOT(onEditBtnClicked()));

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
    //setDevTableWidget();
    initDevListWidget();
    emit sendMac(dinfo.name);
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
    db_connect.send_query("DELETE FROM host WHERE host_id = " + std::to_string(host_id_));

    ui->devInfo->clear();
    ui->devTable->clear();
    setDevTableWidget();
}

