#ifndef DTHREAD_H
#define DTHREAD_H

#include <QObject>
#include <QThread>

class DThread : public QThread
{
    Q_OBJECT
public:
    explicit DThread(QObject *parent = 0);

private slots:
    void isConnect(std::string);

private:
    std::string mac;
    QWidget *dWidget;
    void run() override;
};

#endif // DTHREAD_H
