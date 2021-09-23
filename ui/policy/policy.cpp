#include "policy.h"
#include "ui_policy.h"

policy::policy(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::policy)
{
    ui->setupUi(this);
}

policy::~policy()
{
    delete ui;
}

