#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include "dinfo.h"
#include "../../app/db-connect/db-connect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setDummyDB();
    void setDevInfo();
    void setTableView();
    void clear_devices();

private slots:
    void on_devTable_cellClicked(int row, int column);
    void on_researchBtn_clicked();
    void on_DeleteBtn_clicked();

    void on_policyBtn_clicked();

private:
    Ui::Widget *ui;
    std::vector<dInfo> devices;
    dInfo dinfo;
    DB_Connect *db_connect;
};
#endif // WIDGET_H
