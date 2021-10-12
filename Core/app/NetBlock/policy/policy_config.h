#ifndef POLICY_CONFIG_H
#define POLICY_CONFIG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>
#include "policyobj.h"

namespace Ui {
class policy_config;
}

class policy_config : public QDialog
{
    Q_OBJECT

public:
    explicit policy_config(QModelIndexList indexList, int policyId = 0, QWidget *parent = nullptr);
    ~policy_config();

    PolicyObj policy_obj;

    void getHostListFromDatabase();

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

    void on_hostList_itemSelectionChanged();

    void on_deleteButton_clicked();

private:
    Ui::policy_config *ui;
    QStringList colorList = { "#fa5252", "#be4bdb", "#4c6ef5", "#82c91e", "#ffa8a8", "#e599f7", "#91a7ff", "#c0eb75", "#c92a2a", "#862e9c", "#862e9c", "#5c940d" };
};

#endif // POLICY_CONFIG_H
