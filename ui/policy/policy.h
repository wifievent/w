#ifndef POLICY_H
#define POLICY_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class policy; }
QT_END_NAMESPACE

class policy : public QDialog
{
    Q_OBJECT

public:
    policy(QWidget *parent = nullptr);
    ~policy();

private:
    Ui::policy *ui;
};
#endif // POLICY_H
