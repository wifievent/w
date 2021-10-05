#ifndef NETBLOCK_H
#define NETBLOCK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NetBlock; }
QT_END_NAMESPACE

class NetBlock : public QMainWindow
{
    Q_OBJECT

public:
    NetBlock(QWidget *parent = nullptr);
    ~NetBlock();

public slots:
    void receiveMac(const QString mac);

private:
    Ui::NetBlock *ui;
};
#endif // NETBLOCK_H
