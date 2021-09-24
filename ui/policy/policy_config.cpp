#include "policy_config.h"
#include "ui_policy_config.h"

policy_config::policy_config(QModelIndexList indexList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::policy_config)
{
    ui->setupUi(this);

    QRegularExpression exp("day_of_week_[0-6]");
    QList<QCheckBox *>day_of_week_selected = findChildren<QCheckBox *>(exp);
    for(QModelIndex itm : indexList) {
        day_of_week_selected[itm.column()]->setChecked(true);
    }

    QTime start_time = QTime(indexList.constFirst().row(), 0);
    QTime end_time = QTime(indexList.constLast().row() + 1, 0);
    ui->start_time->setTime(start_time);
    ui->end_time->setTime(end_time);
}

policy_config::~policy_config()
{
    delete ui;
}
