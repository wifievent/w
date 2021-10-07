#ifndef NETBLOCK_H
#define NETBLOCK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class NetBlock_UI; }
QT_END_NAMESPACE

class NetBlock_UI : public QMainWindow
{
    Q_OBJECT

public:
    NetBlock_UI(QWidget *parent = nullptr);
    ~NetBlock_UI();

private:
    Ui::NetBlock_UI *ui;
};
#endif // NETBLOCK_H
