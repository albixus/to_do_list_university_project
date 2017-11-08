#include "add.h"
#include "ui_add.h"
#include "task.h"
#include <iostream>

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

    if(QDate::currentDate().daysTo(ui->calendar->selectedDate())<0)
    {
          QMessageBox::about(this,"BŁĄD","Zmień date. Zaznaczyłeś date w przeszłości!");
          return;
    }
    if(!(ui->add_task->isModified()))
    {
        QMessageBox::about(this,"ERROR","Musisz podać treść swojego zadania");
        return;
    }

        tmp_task.set_priority(ui->priority_spinBox->value());
        tmp_task.set_text(ui->add_task->text());
        tmp_task.set_date(ui->calendar->selectedDate());
        write_to_file("all_tasks.tsk");
        Add::close();
}

void Add::on_cancel_button_clicked()
{
    Add::close();
}

void Add::write_to_file(std::string filename)
{
    file.open(filename,std::ios::out|std::ios::app);

    if(file.is_open())
    {
            file<<tmp_task.get_complete_text().toStdString()<<std::endl;
    }
    else
    {
        QMessageBox::about(this,"ERROR","Cannot write to file");
    }

        file.close();
}



void Add::on_calendar_activated(const QDate &date)
{
//empty slot becouse there were errors in .moc
}
