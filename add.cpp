#include "add.h"
#include "ui_add.h"
#include "task.h"

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
    ui->calendar->setSelectedDate(QDate::currentDate());
}

Add::~Add()
{
    delete ui;
}

void Add::on_ok_button_clicked()
{
    tmp_task.set_priority(ui->priority_spinBox->value());
    tmp_task.set_text(ui->add_task->text());    
    tmp_task.set_date(ui->calendar->selectedDate());
    task_vector.push_back(tmp_task);

    Add::close();
}

void Add::on_cancel_button_clicked()
{
    Add::close();
}

void Add::on_calendar_activated(const QDate &date)
{

}


