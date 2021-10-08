#include "dthread.h"
#include <QDebug>

DThread::DThread(QObject *parent) :
    QThread(parent)
{
}

void DThread::init(QWidget *widget)
{
    this->dWidget = widget;
}

void DThread::run()
{
    // fs
    // widget pointer
    // ui access x, only modify vector
    // thread switching
    // working thread ui access
    int i = 0;
    while(i < 10) {
        //for(int i = 0; i < dWidget->devices.size(); i++) {
          //  mac = devices[i]
            QString thQmac = QString::fromStdString(mac);
            qDebug() << "(DThread run)mac :" << thQmac;
            i++;
        //}

    }
}

void DThread::isConnect(std::string mac_)
{
    mac = mac_;
    QString qmac_ = QString::fromStdString(mac_);
    qDebug() << "(isConnect)Receive mac : " << qmac_;
}
