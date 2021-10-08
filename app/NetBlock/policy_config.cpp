#include "policy_config.h"
#include "ui_policy_config.h"
#include "base/db-connect.h"

void policy_config::getHostFromDatabase()
{
    DB_Connect& dbConnect = DB_Connect::getInstance();

    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");

    int idx = 0;
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int j = 0; j < iter->argc / 2; j++) {
            ui->listWidget->addItem(iter->argv[1 + j * 2]);
            ui->listWidget->item(idx)->setData(Qt::UserRole, iter->argv[0 + j * 2]);
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

    QRegularExpression exp("day_of_week_[0-6]");
    QList<QCheckBox *>selected_day_of_week_list = findChildren<QCheckBox *>(exp);
    for(QModelIndex itm : indexList) {
        selected_day_of_week_list[itm.column() / 5]->setChecked(true);
    }

    QTime start_time = QTime(indexList.constFirst().row() / 6, indexList.constFirst().row() * 10 % 60);
    QTime end_time = QTime((indexList.constLast().row() + 1) / 6, (indexList.constLast().row() + 1) * 10 % 60);
    ui->start_time->setTime(start_time);
    ui->end_time->setTime(end_time);

    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    getHostFromDatabase();

    if (policyId != 0) {
        DB_Connect& dbConnect = DB_Connect::getInstance();

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

    QList<QListWidgetItem *> selected_host_list = ui->listWidget->selectedItems();
    if (selected_host_list.length() == 0) {
        QMessageBox::information(this, "Warning", "Please select the host to assign");
        return;
    }
    qDebug() << selected_host_list;


    QList<QString>timeIdList;
    std::list<Data_List> dl;
    DB_Connect& dbConnect = DB_Connect::getInstance();
    for (QList<int>::iterator iter = checked_day_of_week.begin(); iter != checked_day_of_week.end(); ++iter) {
        QString query = "INSERT INTO time VALUES(null, '" + start_time + "', '" + end_time + "', " + QString::number(*iter) + ")";
        dbConnect.send_query(query.toStdString());
        query = "SELECT seq FROM sqlite_sequence WHERE name='time'";

        dl = dbConnect.select_query(query.toStdString());
        for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
            timeIdList.append(QString(iter->argv[0]));
        }
    }

    for (QList<QListWidgetItem *>::iterator host = selected_host_list.begin(); host != selected_host_list.end(); ++host) {
        for (QList<QString>::iterator time = timeIdList.begin(); time != timeIdList.end(); ++time) {
            QString query = "INSERT INTO policy VALUES(null, " + (*host)->data(Qt::UserRole).toString() + ", " + *time + ")";
            dbConnect.send_query(query.toStdString());
        }
    }

    close();
}

void policy_config::on_cancelButton_clicked()
{
    close();
}
