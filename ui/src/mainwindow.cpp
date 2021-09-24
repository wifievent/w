#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testSlot()
{
    qDebug() << "Click Slot 실행";
}

void MainWindow::showPopup()
{
    PopupDialog popup;
    popup.setModal(true);
    popup.exec();
}

void MainWindow::on_ruleBtn_clicked()
{
    showPopup();
}


void MainWindow::on_deviceBtn_clicked()
{
    showPopup();
}

