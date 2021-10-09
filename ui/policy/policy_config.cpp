#include "policy_config.h"
#include "ui_policy_config.h"
#include "../app/db-connect/db-connect.h"

void policy_config::getHostListFromDatabase()
{
    DB_Connect dbConnect("test.db");

    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");

    int idx = 0;
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int j = 0; j < iter->argc / 2; j++) {
            QColor mColor(colorList[atoi(iter->argv[0 + j * 2]) - 1]);
            ui->hostList->addItem(iter->argv[1 + j * 2]);
            ui->hostList->item(idx)->setForeground(mColor);
            ui->hostList->item(idx)->setData(Qt::UserRole, iter->argv[0 + j * 2]);
        }
        idx++;
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
    ui->hostList->setSelectionMode(QAbstractItemView::SingleSelection);

    getHostListFromDatabase();

    int day_of_the_week = -1;
    QTime start_time;
    QTime end_time;
    if (!policyId) {
        day_of_the_week = indexList.constFirst().column() / 5;
        start_time = QTime(indexList.constFirst().row() / 2, indexList.constFirst().row() * 30 % 60);
        end_time = QTime((indexList.constLast().row() + 1) / 2, (indexList.constLast().row() + 1) * 30 % 60);
    } else {
        ui->hostList->setDisabled(true);
        ui->dayOfTheWeekLayout->setDisabled(true);
        ui->deleteButton->setDisabled(false);

        DB_Connect dbConnect("test.db");
        std::list<Data_List> dl;
        dl = dbConnect.select_query("SELECT p.host_id, t.start_time, t.end_time, t.day_of_the_week FROM policy AS p JOIN time AS t ON t.time_id=p.time_id WHERE p.policy_id= " + QString::number(policyId).toStdString());

        for (std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
            day_of_the_week = atoi(iter->argv[3]);

            int start_hour = QString(iter->argv[1]).leftRef(2).toInt();
            int start_minute = QString(iter->argv[1]).rightRef(2).toInt();
            int end_hour = QString(iter->argv[2]).leftRef(2).toInt();
            int end_minute = QString(iter->argv[2]).rightRef(2).toInt();
            start_time = QTime(start_hour, start_minute);
            end_time = QTime(end_hour, end_minute);

            int host_id = atoi(iter->argv[0]);
            for (int j = 0; j < ui->hostList->count(); j++) {
                if (ui->hostList->item(j)->data(Qt::UserRole) == host_id) {
                    ui->hostList->item(j)->setSelected(true);
                }
            }
        }
    }

    QList<QAbstractButton *> dayOfTheWeekList = ui->dayOfTheWeekGroup->buttons();
    for (QAbstractButton *itm : qAsConst(dayOfTheWeekList)) {
        int day_id = itm->objectName().split('_').last().toInt();
        ui->dayOfTheWeekGroup->setId(itm, day_id);
        if (day_id == day_of_the_week) {
            itm->setChecked(true);
        }
    }

    ui->start_time->setTime(start_time);
    ui->end_time->setTime(end_time);
}

policy_config::~policy_config()
{
    delete ui;
}

void policy_config::on_applyButton_clicked()
{
    int policy_id = this->windowTitle().toInt();
    int day_of_the_week = ui->dayOfTheWeekGroup->checkedId();

    QString start_time = QString("%1%2")
                                .arg(ui->start_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->start_time->time().minute(), 2, 10, QLatin1Char('0'));
    QString end_time = QString("%1%2")
                                .arg(ui->end_time->time().hour(), 2, 10, QLatin1Char('0'))
                                .arg(ui->end_time->time().minute(), 2, 10, QLatin1Char('0'));

    QListWidgetItem *selected_host = ui->hostList->selectedItems().constFirst();
    QList<QListWidgetItem *> selected_host_list = ui->hostList->selectedItems();

    std::list<Data_List> dl;
    DB_Connect dbConnect("test.db");

    int time_id = 0;

    QString query;

    if (!policy_id) {
        if (start_time <= end_time) {
            query = "SELECT count(p.policy_id) \
                    FROM policy AS p \
                        JOIN time AS t \
                            ON t.time_id=p.time_id \
                    WHERE p.host_id=" + selected_host->data(Qt::UserRole).toString() + " \
                        AND t.day_of_the_week=" + QString::number(day_of_the_week) + " \
                        AND (\
                            (\
                                ('" + start_time + "' BETWEEN t.start_time AND t.end_time \
                                    OR t.start_time BETWEEN '" + start_time + "' AND '" + end_time + "') \
                                AND t.start_time <= t.end_time) \
                            OR (\
                                ("
"                                   ('" + start_time + "' BETWEEN t.start_time AND '2400' \
                                        OR '" + start_time + "' BETWEEN '0000' AND t.end_time) \
                                    OR t.start_time BETWEEN '" + start_time + "' AND '" + end_time + "') \
                                AND t.start_time > t.end_time) \
                            )";
            dl = dbConnect.select_query(query.toStdString());
            if (atoi(dl.begin()->argv[0])) {
                QMessageBox::warning(this, tr("Error"),
                                               tr("The policy time overlaps other policy time.\n"
                                                  "Please check other polcy time."),
                                               QMessageBox::Close);
                return;
            }
        } else {
            query = "SELECT count(p.policy_id) \
                    FROM policy AS p \
                        JOIN time AS t \
                            ON t.time_id=p.time_id \
                    WHERE p.host_id=" + selected_host->data(Qt::UserRole).toString() + " \
                        AND t.day_of_the_week=" + QString::number(day_of_the_week) + " \
                        AND (\
                            ('" + start_time + "' BETWEEN t.start_time AND t.end_time \
                                OR '" + end_time + "' BETWEEN t.start_time AND t.end_time) \
                            AND t.start_time <= t.end_time) \
                        OR t.start_time > t.end_time";
            dl = dbConnect.select_query(query.toStdString());
            if (atoi(dl.begin()->argv[0])) {
                QMessageBox::warning(this, tr("Error"),
                                               tr("The policy time overlaps other policy time.\n"
                                                  "Please check other polcy time."),
                                               QMessageBox::Close);
                return;
            }
        }
    }

    query = "INSERT INTO time VALUES(null, '" + start_time + "', '" + end_time + "', " + QString::number(day_of_the_week) + ")";
    dbConnect.send_query(query.toStdString());

    query = "SELECT seq FROM sqlite_sequence WHERE name='time'";
    dl = dbConnect.select_query(query.toStdString());
    time_id = atoi(dl.begin()->argv[0]);

    if (policy_id) {
        query = "UPDATE policy SET time_id=" + QString::number(time_id) + " WHERE policy_id=" + QString::number(policy_id);
        dbConnect.send_query(query.toStdString());
    } else {
        for (QList<QListWidgetItem *>::iterator host = selected_host_list.begin(); host != selected_host_list.end(); ++host) {
            query = "INSERT INTO policy VALUES(null, " + (*host)->data(Qt::UserRole).toString() + ", " + QString::number(time_id) + ")";
            dbConnect.send_query(query.toStdString());
        }
    }
    accept();
    close();
}

void policy_config::on_cancelButton_clicked()
{
    close();
}

void policy_config::on_hostList_itemSelectionChanged()
{
    if (ui->hostList->selectedItems().length()) {
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
    accept();
    close();
}

