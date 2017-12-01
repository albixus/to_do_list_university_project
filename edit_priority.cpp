#include "edit_priority.h"
#include "ui_edit_priority.h"

Edit_priority::Edit_priority(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_priority)
{
    ui->setupUi(this);
}

Edit_priority::~Edit_priority()
{
    delete ui;
}

void Edit_priority::on_ok_button_clicked()
{
    priority = ui->priority_change->value();
    Edit_priority::close();
}

void Edit_priority::on_cancel_button_clicked()
{
    priority=-1;
    Edit_priority::close();
}
