#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include "dinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setDevInfo();
    void setTableView();

private slots:
    void on_devTable_cellClicked(int row, int column);

private:
    Ui::Widget *ui;
    std::vector<dInfo> devices;
};
#endif // WIDGET_H
