#ifndef NETBLOCK_UI_H
#define NETBLOCK_UI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NetBlock; }
QT_END_NAMESPACE

class NetBlock_UI : public QMainWindow
{
    Q_OBJECT

public:
    NetBlock_UI(QWidget *parent = nullptr);
    ~NetBlock_UI();

public slots:
    void receiveMac(const QString mac);

private:
    Ui::NetBlock *ui;
};
#endif // NETBLOCK_UI_H
