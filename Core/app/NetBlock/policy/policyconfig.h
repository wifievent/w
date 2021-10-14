#ifndef POLICYCONFIG_H
#define POLICYCONFIG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include "policyobj.h"
#include "base/db-connect.h"

namespace Ui {
class PolicyConfig;
}

class PolicyConfig : public QDialog
{
    Q_OBJECT

public:
    explicit PolicyConfig(QModelIndexList indexList, int policyId, int hostId, QDialog *parent = nullptr);
    ~PolicyConfig();

    PolicyObj policy_obj;
    DB_Connect& dbConnect = DB_Connect::getInstance();

    void getHostListFromDatabase(int selectedHostId);

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

    void on_hostList_itemSelectionChanged();

    void on_deleteButton_clicked();

private:
    Ui::PolicyConfig *ui;
    QStringList colorList = { "#fa5252", "#be4bdb", "#4c6ef5", "#82c91e", "#ffa8a8", "#e599f7", "#91a7ff", "#c0eb75", "#c92a2a", "#862e9c", "#862e9c", "#5c940d" };
};

#endif // POLICYCONFIG_H
