#include "task.h"


std::vector<task> task_vector;

task::task()
{
    date=QDate(2017,1,1);
    priority=0;
    text=QString("");
    context_menu = new QMenu();
    context_menu->addAction("Edytuj");
    context_menu->addAction("Usuń");
}

void task::set_date(QDate date)
{
    this->date=date;
}

void task::set_priority(int priority)
{
    if(priority<0||priority>3)
        return;
    this->priority=priority;
}

void task::set_text(QString text)
{
    this->text=text;
}
