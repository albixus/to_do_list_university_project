#include "accept.h"
#include "ui_accept.h"

accept::accept(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accept)
{
    ui->setupUi(this);
}

accept::~accept()
{
    delete ui;
}
