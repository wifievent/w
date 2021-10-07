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

    void on_listWidget_itemSelectionChanged();

    void on_deleteButton_clicked();

private:
    Ui::policy_config *ui;
    QStringList colorList = { "#ff6b6b", "#339af0", "#51cf66", "#ff922b" };
};

#endif // POLICY_CONFIG_H
