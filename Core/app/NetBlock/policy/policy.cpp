#include "policy.h"
#include "ui_policy.h"
#include "policy_config.h"

int policy::checkOverlapCell(int startRow, int endRow, int columnIndex)
{
    for (int i = 0; i < 48; i++) {
        if (ui->tableWidget->item(i, columnIndex) != nullptr) {
            int prev_start_row = i;
            int prev_end_row = i + ui->tableWidget->rowSpan(i, columnIndex);
            if (endRow > prev_start_row and startRow < prev_end_row) {
                ui->tableWidget->showColumn(columnIndex + 1);
                return 1 + checkOverlapCell(startRow, endRow, columnIndex + 1);
            }
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
}

void policy::setItmPolicy(int row, int column, QColor policyColor, int policyId)
{
    QTableWidgetItem *itm = new QTableWidgetItem();
    itm->setBackground(policyColor);
    ui->tableWidget->setItem(row, column, itm);
    itm->setData(Qt::UserRole, policyId);
}

void policy::getHostFromDatabase()
{
    QVariantList selectedId;
    if (selectedHost.length()) {
        for (QList<QListWidgetItem *>::iterator iter = selectedHost.begin(); iter != selectedHost.end(); ++iter) {
            selectedId.append((*iter)->data(Qt::UserRole));
        }
    }
    ui->host_filter->clear();

    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");

    int idx = 0;
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        QListWidgetItem *host_filter_item = new QListWidgetItem(QString::fromStdString(iter->argv[1]));
        QColor mColor(colorList[(stoi(iter->argv[0]) - 1) % colorList.length()]);
        host_filter_item->setBackground(mColor);
        host_filter_item->setData(Qt::UserRole, QString::fromStdString(iter->argv[0]));
        ui->host_filter->addItem(host_filter_item);

        for (QVariantList::iterator iter = selectedId.begin(); iter != selectedId.end(); ++iter) {
            if (host_filter_item->data(Qt::UserRole).toString() == (*iter).toString()) {
                host_filter_item->setSelected(true);
            }
        }
        idx++;
    }
}

void policy::getPolicyFromDatabase(QString where)
{
    policyList.clear();
    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT p.policy_id, t.start_time, t.end_time, t.day_of_the_week, h.host_id, h.name \
                                FROM policy AS p \
                                JOIN time as t \
                                    ON t.time_id=p.time_id \
                                JOIN host AS h \
                                    ON h.host_id=p.host_id \
                                " + where.toStdString() + " ORDER BY t.day_of_the_week ASC");
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        policyObj.reset();
        policyObj.policyId = stoi(iter->argv[0]);
        policyObj.start_time = QString::fromStdString(iter->argv[1]);
        policyObj.end_time = QString::fromStdString(iter->argv[2]);
        policyObj.day_of_the_week = stoi(iter->argv[3]);
        policyObj.hostId = stoi(iter->argv[4]);
        policyObj.name = QString::fromStdString(iter->argv[5]);
        policyList.append(policyObj);
    }
}

void policy::setPolicyToTable()
{
    resetPolicyTable();
    int idx = 0;
    for (QVector<PolicyObj>::iterator iter = policyList.begin(); iter != policyList.end(); ++iter) {
        int start_hour = iter->start_time.leftRef(2).toInt();
        int start_min = iter->start_time.rightRef(2).toInt();
        int end_hour = iter->end_time.leftRef(2).toInt();
        int end_min = iter->end_time.rightRef(2).toInt();
        QColor mColor(colorList[(iter->hostId - 1) % colorList.length()]);

        int startRow = start_hour * 2 + start_min / 30;
        int endRow = end_hour * 2 + end_min / 30;
        int columnIndex = iter->day_of_the_week * 5;
        if (iter->start_time <= iter->end_time) {
            int overlapCellCount = checkOverlapCell(startRow, endRow, columnIndex);
            if (overlapCellCount) {
                for (int i = 0; i < 5; i++) {
                    ui->tableWidget->setColumnWidth(columnIndex + i, 100 / (overlapCellCount + 1));
                }
            }
            columnIndex += overlapCellCount;
            setItmPolicy(startRow, columnIndex, mColor, iter->policyId);
            if (endRow - startRow != 1) {
                ui->tableWidget->setSpan(startRow, columnIndex, endRow - startRow, 1);
            }
        } else {
            // overlap check
            setItmPolicy(startRow, columnIndex, mColor, iter->policyId);
            ui->tableWidget->setSpan(startRow, columnIndex, 24 * 2 - startRow, 1);
            setItmPolicy(0, columnIndex, mColor, iter->policyId);
            ui->tableWidget->setSpan(0, columnIndex, end_hour * 2 + end_min / 30, 1);
        }
        idx++;
    }
}

void policy::openPolicyConfig()
{
    policy_config *policyConfig;
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty()) {
        QTableWidgetItem *firstItem = ui->tableWidget->item(indexList.constFirst().row(), indexList.constFirst().column());
        if (firstItem == nullptr) {
            policyConfig = new policy_config(indexList);
        } else {
            policyConfig = new policy_config(indexList, firstItem->data(Qt::UserRole).toInt());
        }
    } else {
        policyConfig = new policy_config(indexList);
    }
    policyConfig->setModal(true);

    int result = policyConfig->exec();

    if (result == QDialog::Accepted) {
        getPolicyFromDatabase();
        setPolicyToTable();
        ui->tableWidget->clearSelection();
    }
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

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(getHostFromDatabase()));
    timer->start(10000);

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

void policy::on_tableWidget_itemSelectionChanged()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 1 && selectedIndexes.constFirst().column() != selectedIndexes.constLast().column()) {
        ui->tableWidget->clearSelection();
    }
}
