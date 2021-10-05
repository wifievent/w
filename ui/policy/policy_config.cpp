#include "policy_config.h"
#include "ui_policy_config.h"
#include "../app/db-connect/db-connect.h"

void policy_config::getHostFromDatabase()
{
    DB_Connect dbConnect("test.db");

    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");

    int idx = 0;
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int j = 0; j < iter->argc / 2; j++) {
            QColor mColor(colorList[atoi(iter->argv[0 + j * 2]) - 1]);
            ui->listWidget->addItem(iter->argv[1 + j * 2]);
            ui->listWidget->item(idx)->setForeground(mColor);
            ui->listWidget->item(idx)->setData(Qt::UserRole, iter->argv[0 + j * 2]);
        }
        idx++;
    }
}

void policy_config::setPolicyConfigModal(int policyId) {
    ui->listWidget->setDisabled(true);
    ui->dayOfTheWeekLayout->setDisabled(true);
    ui->deleteButton->setDisabled(false);

    DB_Connect dbConnect("test.db");
    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id FROM policy WHERE policy_id= " + QString::number(policyId).toStdString());

    for (std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for (int i = 0; i < iter->argc; i++) {
            for (int j = 0; j < ui->listWidget->count(); j++) {
                if (ui->listWidget->item(j)->data(Qt::UserRole) == iter->argv[i]) {
                    ui->listWidget->item(j)->setSelected(true);
                }
            }
        }
    }
}

policy_config::policy_config(QModelIndexList indexList, int policyId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::policy_config)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::number(policyId));
    ui->applyButton->setDisabled(true);
    ui->deleteButton->setDisabled(true);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);


    QList<QAbstractButton *> dayOfTheWeekList = ui->dayOfTheWeekGroup->buttons();
    for (QAbstractButton *itm : dayOfTheWeekList) {
        int day_id = itm->objectName().split('_').last().toInt();
        ui->dayOfTheWeekGroup->setId(itm, day_id);
        if (day_id == indexList.constFirst().column() / 5) {
            itm->setChecked(true);
        }
    }

    // TODO: if policyId, change DB policy info

    QTime start_time = QTime(indexList.constFirst().row() / 6, indexList.constFirst().row() * 10 % 60);
    QTime end_time = QTime((indexList.constLast().row() + 1) / 6, (indexList.constLast().row() + 1) * 10 % 60);
    ui->start_time->setTime(start_time);
    ui->end_time->setTime(end_time);

    getHostFromDatabase();

    policyId ? setPolicyConfigModal(policyId) : void();
}

policy_config::~policy_config()
{
    delete ui;
}

void policy_config::on_applyButton_clicked()
{
    int policy_id = this->windowTitle().toInt();
    int day_of_the_week = ui->dayOfTheWeekGroup->checkedId();
    qDebug() << policy_id << day_of_the_week;

    QString start_time = QString("%1%2")
                                .arg(ui->start_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->start_time->time().minute(), 2, 10, QLatin1Char('0'));
    QString end_time = QString("%1%2")
                                .arg(ui->end_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->end_time->time().minute(), 2, 10, QLatin1Char('0'));
    qDebug() << start_time << end_time;

    QList<QListWidgetItem *> selected_host_list = ui->listWidget->selectedItems();

    std::list<Data_List> dl;
    DB_Connect dbConnect("test.db");

    int time_id = 0;

    QString query = "INSERT INTO time VALUES(null, '" + start_time + "', '" + end_time + "', " + QString::number(day_of_the_week) + ")";
    dbConnect.send_query(query.toStdString());

    query = "SELECT seq FROM sqlite_sequence WHERE name='time'";
    dl = dbConnect.select_query(query.toStdString());
    time_id = atoi(dl.begin()->argv[0]);

    if (policy_id) {
        // TODO: check already exist time
        // TODO: check duplicated time and policy. if host is same and time is duplicated, merge
        query = "UPDATE policy SET time_id=" + QString::number(time_id) + " WHERE policy_id=" + QString::number(policy_id);
        dbConnect.send_query(query.toStdString());
    } else {
        for (QList<QListWidgetItem *>::iterator host = selected_host_list.begin(); host != selected_host_list.end(); ++host) {
            query = "INSERT INTO policy VALUES(null, " + (*host)->data(Qt::UserRole).toString() + ", " + QString::number(time_id) + ")";
            dbConnect.send_query(query.toStdString());
        }
    }

    close();
}

void policy_config::on_cancelButton_clicked()
{
    close();
}

void policy_config::on_listWidget_itemSelectionChanged()
{
    if (ui->listWidget->selectedItems().length()) {
        ui->applyButton->setEnabled(true);
    } else {
        ui->applyButton->setEnabled(false);
        // if there isn't policy change point, disable apply button
        // TODO
    }
}

void policy_config::on_deleteButton_clicked()
{
    int policy_id = this->windowTitle().toInt();
    DB_Connect dbConnect("test.db");
    QString query = "DELETE FROM policy WHERE policy_id=" + QString::number(policy_id);
    dbConnect.send_query(query.toStdString());
    close();
}

