#ifndef POLICY_H
#define POLICY_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include <QMap>
#include "../app/db-connect/db-connect.h"
#include "policyobj.h"

QT_BEGIN_NAMESPACE
namespace Ui { class policy; }
QT_END_NAMESPACE

class policy : public QDialog
{
    Q_OBJECT

public:
    policy(QWidget *parent = nullptr);
    ~policy();

    enum TABLE_INFO {
        COLUMN_SIZE=5,
        ROW_SIZE=6
    };

    DB_Connect dbConnect = DB_Connect("test.db");
    PolicyObj policyObj;
    QVector<PolicyObj> policyList;
    QList<QListWidgetItem *> selectedHost;
    int checkOverlapCell(int startRow, int lastRow, int columnIdx);
    void resetPolicyTable();
    int setItmPolicy(QTableWidget *tableWidget, int row, int column, QColor policyColor, int policyCnt, int policyId);
    void getPolicyFromDatabase(QString where = "");
    void setPolicyToTable();
    void getHostFromDatabase();

private slots:

    void on_addPolicyButton_clicked();

    void on_tableWidget_cellDoubleClicked();

    void openPolicyConfig();

    void on_host_filter_itemSelectionChanged();


    void on_tableWidget_itemSelectionChanged();

private:
    Ui::policy *ui;
    QStringList colorList = { "#ff6b6b", "#339af0", "#51cf66", "#ff922b" };
};
#endif // POLICY_H
