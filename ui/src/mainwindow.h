#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include "popupdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ruleBtn_clicked();
    void on_deviceBtn_clicked();

private:
    Ui::MainWindow *ui;
    void showPopup();
    void testSlot();
};
#endif // MAINWINDOW_H
