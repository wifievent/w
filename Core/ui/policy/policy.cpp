#include "policy.h"
#include "ui_policy.h"
#include "policy_config.h"

int policy::checkOverlapCell(int startRow, int lastRow, int columnIdx)
{
    for (int i = startRow; i <= lastRow; i++) {
        if (ui->tableWidget->item(i, columnIdx) != nullptr) {
            return 1 + checkOverlapCell(startRow, lastRow, columnIdx + 1);
        }
    }
    return 0;
}

void policy::resetPolicyTable()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->setItem(i, j, nullptr);
            ui->tableWidget->setColumnWidth(j, 100);
            ui->tableWidget->clearSpans();
        }
    }
    for (int i = 0; i < ui->tableWidget->columnCount(); i++) {
        if (i % 5 != 0) {
            ui->tableWidget->hideColumn(i);
        }
    }
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if (i % 6 != 0) {
            //ui->tableWidget->hideRow(i);
        }
    }
}

int policy::setItmPolicy(QTableWidget *tableWidget, int row, int column, QColor policyColor, int policyCnt, int policyId)
{
    if (tableWidget->item(row, column + policyCnt) == nullptr) {
        QTableWidgetItem *itm = new QTableWidgetItem();
        itm->setBackground(policyColor);
        tableWidget->setItem(row, column + policyCnt, itm);
        itm->setData(Qt::UserRole, policyId);
        return column + policyCnt;
    } else {
        tableWidget->showColumn(column + policyCnt + 1);
        for (int i = 0; i < policy::COLUMN_SIZE; i++) {
            tableWidget->setColumnWidth(column + i, 100 / (policyCnt + 2));
        }
        return setItmPolicy(tableWidget, row, column, policyColor, policyCnt + 1, policyId);
    }
}

void policy::getPolicyFromDatabase(QString where)
{
    policyList.clear();
    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT p.policy_id, t.start_time, t.end_time, t.day_of_the_week, h.host_id, h.name FROM policy AS p JOIN time as t ON t.time_id=p.time_id JOIN host AS h ON h.host_id=p.host_id " + where.toStdString() + " ORDER BY t.day_of_the_week ASC");
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc / 6; ++i) {
            policyObj.reset();
            policyObj.policyId = atoi(iter->argv[0 + i * 5]);
            policyObj.start_time = iter->argv[1 + i * 5];
            policyObj.end_time = iter->argv[2 + i * 5];
            policyObj.day_of_the_week = atoi(iter->argv[3 + i * 5]);
            policyObj.hostId = atoi(iter->argv[4 + i * 5]);
            policyObj.name = iter->argv[5 + i * 5];
            policyList.append(policyObj);
        }
    }
}

void policy::getHostFromDatabase()
{
    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");

    int idx = 0;
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int j = 0; j < iter->argc / 2; j++) {
            QColor mColor(colorList[atoi(iter->argv[0 + j * 2]) - 1]);
            ui->host_filter->addItem(iter->argv[1 + j * 2]);
            ui->host_filter->item(idx)->setForeground(mColor);
            ui->host_filter->item(idx)->setData(Qt::UserRole, iter->argv[0 + j * 2]);
        }
        idx++;
    }
}

void policy::setPolicyToTable()
{
    resetPolicyTable();
    for (QVector<PolicyObj>::iterator iter = policyList.begin(); iter != policyList.end(); ++iter) {
        int start_hour = iter->start_time.leftRef(2).toInt();
        int start_min = iter->start_time.rightRef(2).toInt();
        int end_hour = iter->end_time.leftRef(2).toInt();
        int end_min = iter->end_time.rightRef(2).toInt();
        QColor mColor(colorList[iter->hostId % colorList.length() - 1]);

        int startRow = start_hour * 6 + start_min / 10;
        int endRow = end_min / 10;
        int columnIdx = iter->day_of_the_week * 5;
        if (start_hour <= end_hour) {
            int lastRow = startRow + (end_hour - start_hour) * 6 + endRow - 1;
            columnIdx += checkOverlapCell(startRow, lastRow, columnIdx);
            if ((end_hour - start_hour) * 6 + endRow != 1) {
                ui->tableWidget->setSpan(startRow,
                                     setItmPolicy(ui->tableWidget, startRow, columnIdx, mColor, 0, iter->policyId),
                                     (end_hour - start_hour) * 6 + endRow, 1);
            } else {
                setItmPolicy(ui->tableWidget, startRow, columnIdx, mColor, 0, iter->policyId);
            }
        } else {
            ui->tableWidget->setSpan(startRow,
                                     setItmPolicy(ui->tableWidget, startRow, iter->day_of_the_week * 5, mColor, 0, iter->policyId),
                                     (24 - start_hour) * 6, 1);
            ui->tableWidget->setSpan(0,
                                     setItmPolicy(ui->tableWidget, 0, iter->day_of_the_week * 5, mColor, 0, iter->policyId),
                                     end_hour * 6 + endRow, 1);
        }
    }
}

void policy::openPolicyConfig()
{
    policy_config *policyConfig;
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedIndexes();
    QTableWidgetItem *firstItem = ui->tableWidget->item(indexList.constFirst().row(), indexList.constFirst().column());
    if (firstItem == nullptr) {
        policyConfig = new policy_config(indexList);
    } else {
        policyConfig = new policy_config(indexList, firstItem->data(Qt::UserRole).toInt());
    }
    policyConfig->setModal(true);
    policyConfig->exec();
}

policy::policy(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::policy)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    ui->splitter->setStretchFactor(0, 5);
    ui->splitter->setStretchFactor(1, 1);

    ui->host_filter->setSelectionMode(QAbstractItemView::MultiSelection);
    this->setWindowTitle("NetBlock");

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tableWidget->resizeRowsToContents();

    getHostFromDatabase();
    getPolicyFromDatabase();
    setPolicyToTable();
}

policy::~policy()
{
    delete ui;
}

void policy::on_addPolicyButton_clicked()
{
    openPolicyConfig();
}

void policy::on_tableWidget_cellDoubleClicked()
{
    openPolicyConfig();
}

void policy::on_host_filter_itemSelectionChanged()
{
    selectedHost = ui->host_filter->selectedItems();
    QString host_filter_condition = "WHERE h.host_id IN (";
    for(QList<QListWidgetItem *>::iterator iter = selectedHost.begin(); iter != selectedHost.end(); ++iter) {
        host_filter_condition.append((*iter)->data(Qt::UserRole).toString());
        if (iter + 1 != selectedHost.end()) {
            host_filter_condition.append(", ");
        }
    }
    host_filter_condition.append(")");

    getPolicyFromDatabase(host_filter_condition);
    setPolicyToTable();
}

