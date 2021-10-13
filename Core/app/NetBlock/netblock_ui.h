#ifndef NETBLOCK_UI_H
#define NETBLOCK_UI_H

#include <QMainWindow>
#include "policy/policy.h"
#include "device/devicewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class NetBlock; }
QT_END_NAMESPACE

class NetBlock_UI : public QMainWindow
{
    Q_OBJECT

public:
    NetBlock_UI(QWidget *parent = nullptr);
    ~NetBlock_UI();
    void readSettings();

public slots:
    void receiveMac(const QString mac);
    void receiveHostSelect(const int hostId);
    void receiveReload();

private:
    DeviceWidget *device_widget;
    Policy *policy;
    void closeEvent(QCloseEvent*);
    Ui::NetBlock *ui;
};
#endif // NETBLOCK_UI_H
