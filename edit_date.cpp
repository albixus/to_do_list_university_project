#include "edit_date.h"
#include "ui_edit_date.h"
#include <QMessageBox>

edit_date::edit_date(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_date)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

edit_date::~edit_date()
{
    delete ui;
}

void edit_date::on_cancel_button_clicked()
{
    active_date.setDate(1000,1,1);
    edit_date::close();
}

void edit_date::on_ok_button_clicked()
{
    active_date = ui->dateEdit->date();
    if(active_date<QDate::currentDate())
    {
        QMessageBox::about(this,"Błąd","Nie możesz ustawić daty w przeszłości");
        return;
    }
    edit_date::close();
}
