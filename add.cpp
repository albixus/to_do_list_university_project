#include "add.h"
#include "ui_add.h"
#include <iostream>

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
    ui->calendar->setSelectedDate(QDate::currentDate());
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(qApp->applicationDirPath()+"/to_do_list.sqlite");
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
        if(db.open())
        {
            QSqlQuery query;
            if(query.exec("INSERT INTO tasks (Priority,Data,Text) "
                        "VALUES(" + QString::number(ui->priority_spinBox->value()) +  ",'" + ui->calendar->selectedDate().toString("yyyy-MM-dd")+ "','"+ui->add_task->text()+"')"))
            {
               ;
            }
            else
                QMessageBox::about(this,"title","Blad przy dodawaniu zdania do bazy");
            db.close();
        }
        else
            QMessageBox::about(this,"ERROR","add.cpp Nie otwarto bazy");

        QSqlDatabase::removeDatabase(qApp->applicationDirPath()+"/to_do_list.sqlite");
        Add::close();
}

void Add::on_cancel_button_clicked()
{
    Add::close();
}

void Add::on_calendar_activated(const QDate &date)
{
//empty slot becouse there were errors in .moc
}
