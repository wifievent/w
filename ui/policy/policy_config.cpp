#include "policy_config.h"
#include "ui_policy_config.h"

policy_config::policy_config(QModelIndexList indexList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::policy_config)
{
    ui->setupUi(this);

    QRegularExpression exp("day_of_week_[0-6]");
    QList<QCheckBox *>selected_day_of_week_list = findChildren<QCheckBox *>(exp);
    for(QModelIndex itm : indexList) {
        selected_day_of_week_list[itm.column()]->setChecked(true);
    }

    QTime start_time = QTime(indexList.constFirst().row(), 0);
    QTime end_time = QTime(indexList.constLast().row() + 1, 0);
    ui->start_time->setTime(start_time);
    ui->end_time->setTime(end_time);

    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // if new policy, condition is false
    bool condition = true;
    if (condition) {
        ui->deleteButton->setDisabled(true);
    }
}

policy_config::~policy_config()
{
    delete ui;
}

void policy_config::on_applyButton_clicked()
{
    QRegularExpression exp("day_of_week_[0-6]");
    QList<QCheckBox *>day_of_week_list = findChildren<QCheckBox *>(exp);
    QList<int>checked_day_of_week;
    for(int i = 0; i < day_of_week_list.length(); i++) {
        if (day_of_week_list[i]->isChecked()) {
            checked_day_of_week.append(i);
        }
    }
    qDebug() << checked_day_of_week;

    QString start_time = QString("%1%2")
                                .arg(ui->start_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->start_time->time().minute(), 2, 10, QLatin1Char('0'));
    QString end_time = QString("%1%2")
                                .arg(ui->end_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->end_time->time().minute(), 2, 10, QLatin1Char('0'));
    qDebug() << start_time << end_time;

    QList<QListWidgetItem*> selected_host_list = ui->listWidget->selectedItems();
    if (selected_host_list.length() == 0) {
        QMessageBox::information(this, "Warning", "Please select the host to assign");
        return;
    }
    qDebug() << selected_host_list;

    close();
}

void policy_config::on_cancelButton_clicked()
{
    close();
}
