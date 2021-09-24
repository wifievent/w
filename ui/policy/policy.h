#ifndef POLICY_H
#define POLICY_H

#include <QDialog>
#include <QtCore>
#include <QtWidgets>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class policy; }
QT_END_NAMESPACE

class policy : public QDialog
{
    Q_OBJECT

public:
    policy(QWidget *parent = nullptr);
    ~policy();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableWidget_cellDoubleClicked();

    void open_policy_config();

private:
    Ui::policy *ui;
};
#endif // POLICY_H
