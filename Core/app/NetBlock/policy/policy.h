#ifndef POLICY_H
#define POLICY_H

#include <QWidget>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include <QMap>
#include "base/db-connect.h"
#include "policyobj.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Policy; }
QT_END_NAMESPACE

class Policy : public QWidget
{
    Q_OBJECT

public:
    Policy(QWidget *parent = nullptr);
    ~Policy();

    enum MINUTE {
        HOUR=60,
    };

    DB_Connect& dbConnect = DB_Connect::getInstance();
    PolicyObj policyObj;
    QVector<PolicyObj> policyList;
    int selectedHostId = 0;

    void resetPolicyTable();
    void setItmPolicy(int row, int column, int policyId, int span);
    void getPolicyFromDatabase(QString where = "");
    void setPolicyToTable();

private slots:
    void on_addPolicyButton_clicked();
    void on_tableWidget_cellDoubleClicked();
    void openPolicyConfig();
    void getHostListFromDatabase();
    void on_hostFilter_activated(int index);

private:
    Ui::Policy *ui;
    QTimer *timer;
};
#endif // POLICY_H
