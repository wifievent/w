#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QItemSelection>
#include <QTimer>
#include <QLineEdit>
#include <QListWidgetItem>
#include "dinfo.h"
#include "../../app/db-connect/db-connect.h"
#include "../../app/hostscan-test/fullscan.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DeviceWidget; }
QT_END_NAMESPACE

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    DeviceWidget(QWidget *parent = nullptr);
    ~DeviceWidget();
    void setDevInfo();
    void setDevTableWidget();
    void initDevListWidget();
    void setListWidgetItem(QString);
    void activateBtn();
    void clear_devices();
    void viewDevState();

signals:
    void sendMac(const QString mac);

private slots:
    void on_devTable_cellClicked(int row, int column);
    void on_reloadBtn_clicked();
    void on_policyBtn_clicked();
    void on_deleteBtn_clicked();
    void slotSelectionChanged();
    void updateDevState();
    void onEditBtnClicked();

private:
    Ui::DeviceWidget *ui;
    dInfo dinfo;
    DB_Connect *db_connect;
    std::vector<dInfo> devices;
    QTimer *timer;
    QLineEdit *lineEdit;
    // FullScan& fs = FullScan::instance_fs();
};
#endif // DEVICEWIDGET_H
