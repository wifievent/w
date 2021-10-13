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
    int selectedPolicyId = 0;
    QString hostFilterCondition = "";

    void resetPolicyTable();
    void setItmPolicy(int row, int column, int policyId, int span);
    void getPolicyFromDatabase(QString where = "");
    void setPolicyToTable();

private slots:
    void openPolicyConfig();
    void getHostListFromDatabase();
    void on_hostFilter_activated();

    void on_addButton_clicked();

    void on_editButton_clicked();

    void on_deleteButton_clicked();

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::Policy *ui;
    QTimer *timer;
};
#endif // POLICY_H
