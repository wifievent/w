#include "policy.h"
#include "ui_policy.h"
#include "policy_config.h"

policy::policy(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::policy)
{
    ui->setupUi(this);
    this->setWindowTitle("NetBlock");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

policy::~policy()
{
    delete ui;
}

void policy::on_pushButton_clicked()
{
    QMessageBox::information(this, "title", "Main");
}

void policy::on_pushButton_2_clicked()
{
    open_policy_config();
}

void policy::on_tableWidget_cellDoubleClicked()
{
    open_policy_config();
}

void policy::open_policy_config()
{
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedIndexes();
    policy_config mDialog(indexList);
    mDialog.setModal(true);
    mDialog.exec();
}
