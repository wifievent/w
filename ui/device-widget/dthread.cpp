#include "dthread.h"
#include <QDebug>

DThread::DThread(QObject *parent) :
    QThread(parent)
{
    dWidget = (QWidget*)parent;
}

void DThread::run()
{
    // fs
    // widget pointer
    // ui access x, only modify vector
    // thread switching
    // working thread ui access
    while(true) {
        //for(int i = 0; i < dWidget->devices.size(); i++) {
          //  mac = devices[i]
            QString thQmac = QString::fromStdString(mac);
            qDebug() << "(DThread run)mac :" << thQmac;
        //}

    }
}

void DThread::isConnect(std::string mac_)
{
    mac = mac_;
    QString qmac_ = QString::fromStdString(mac_);
    qDebug() << "(isConnect)Receive mac : " << qmac_;
    start();
}
