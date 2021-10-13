#include "policy.h"
#include "ui_policy.h"
#include "policyconfig.h"

void Policy::resetPolicyTable()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            ui->tableWidget->setItem(i, j, nullptr);
            ui->tableWidget->setColumnWidth(j, 100);
            ui->tableWidget->clearSpans();
        }
    }
}

void Policy::setItmPolicy(int row, int column, int policyId, int span)
{
    if (span > 1)
        ui->tableWidget->setSpan(row, column, span, 1);

    QTableWidgetItem *itm = new QTableWidgetItem();
    ui->tableWidget->setItem(row, column, itm);
    itm->setData(Qt::UserRole, policyId);
}

void Policy::getHostListFromDatabase()
{
    ui->hostFilter->clear();

    std::list<Data_List> dl;
    dl = dbConnect.select_query("SELECT host_id, name FROM host");
    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        int hostId = stoi(iter->argv[0]);
        QString name = QString::fromStdString(iter->argv[1]);

        ui->hostFilter->addItem(name, hostId);
    }

    if (selectedHostId)
        ui->hostFilter->setCurrentIndex(ui->hostFilter->findData(selectedHostId));
}

void Policy::getPolicyFromDatabase(QString where)
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
        policyObj.set(iter->argv);
        policyList.append(policyObj);
    }
}

void Policy::setPolicyToTable()
{
    resetPolicyTable();
    for (QVector<PolicyObj>::iterator iter = policyList.begin(); iter != policyList.end(); ++iter) {
        int startHour = iter->getStartTime().leftRef(2).toInt();
        int startMin = iter->getStartTime().rightRef(2).toInt();
        int endHour = iter->getEndTime().leftRef(2).toInt();
        int endMin = iter->getEndTime().rightRef(2).toInt();

        int startRow = startHour + startMin / MINUTE::HOUR;
        int endRow = endHour + endMin / MINUTE::HOUR;
        int column = iter->getDayOfTheWeek();
        int span = 0;
        if (iter->getStartTime() <= iter->getEndTime()) {
            span = endRow - startRow;
            setItmPolicy(startRow, column, iter->getPolicyId(), span);
        } else {
            span = 24 - startRow;
            setItmPolicy(startRow, column, iter->getPolicyId(), span);

            span = endRow;
            setItmPolicy(0, column, iter->getPolicyId(), span);
        }
    }
}

void Policy::openPolicyConfig()
{
    PolicyConfig *policyConfig;
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty()) {
        QTableWidgetItem *firstItem = ui->tableWidget->item(indexList.constFirst().row(), indexList.constFirst().column());
        if (firstItem == nullptr) {
            policyConfig = new PolicyConfig(indexList);
        } else {
            policyConfig = new PolicyConfig(indexList, firstItem->data(Qt::UserRole).toInt());
        }
    } else {
        policyConfig = new PolicyConfig(indexList);
    }

    policyConfig->setModal(true);

    int result = policyConfig->exec();

    if (result == QDialog::Accepted) {
        getPolicyFromDatabase();
        setPolicyToTable();
        ui->tableWidget->clearSelection();
    }
}

Policy::Policy(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Policy)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 5);
    ui->splitter->setStretchFactor(1, 1);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->tableWidget->resizeRowsToContents();

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(getHostListFromDatabase()));
    timer->start(10000);

    getHostListFromDatabase();
    getPolicyFromDatabase();
    setPolicyToTable();
}

Policy::~Policy()
{
    delete ui;
}

void Policy::on_addPolicyButton_clicked()
{
    openPolicyConfig();
}

void Policy::on_tableWidget_cellDoubleClicked()
{
    openPolicyConfig();
}

void Policy::on_hostFilter_activated(int index)
{
    selectedHostId = ui->hostFilter->currentData().toInt();
    QString hostFilterCondition = "WHERE h.host_id=" + QString::number(selectedHostId);
        getPolicyFromDatabase(hostFilterCondition);
        setPolicyToTable();
}

