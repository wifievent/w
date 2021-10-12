#ifndef POLICY_H
#define POLICY_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include <QMap>
#include "base/db-connect.h"
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

    DB_Connect& dbConnect = DB_Connect::getInstance();
    PolicyObj policyObj;
    QVector<PolicyObj> policyList;
    QList<QListWidgetItem *> selectedHost;
    int checkOverlapCell(int startRow, int lastRow, int columnIdx);
    void resetPolicyTable();
    void setItmPolicy(int row, int column, QColor policyColor, int policyId);
    void getPolicyFromDatabase(QString where = "");
    void setPolicyToTable();

private slots:
    void on_addPolicyButton_clicked();
    void on_tableWidget_cellDoubleClicked();
    void openPolicyConfig();
    void on_host_filter_itemSelectionChanged();
    void on_tableWidget_itemSelectionChanged();
    void getHostFromDatabase();

private:
    Ui::policy *ui;
    QStringList colorList = { "#fa5252", "#be4bdb", "#4c6ef5", "#82c91e", "#ffa8a8", "#e599f7", "#91a7ff", "#c0eb75", "#c92a2a", "#862e9c", "#862e9c", "#5c940d" };
    QTimer *timer;
};
#endif // POLICY_H
