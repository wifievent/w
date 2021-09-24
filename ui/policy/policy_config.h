#ifndef POLICY_CONFIG_H
#define POLICY_CONFIG_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>

namespace Ui {
class policy_config;
}

class policy_config : public QDialog
{
    Q_OBJECT

public:
    explicit policy_config(QModelIndexList indexList, QWidget *parent = nullptr);
    ~policy_config();

private slots:
    void on_applyButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::policy_config *ui;
};

#endif // POLICY_CONFIG_H
