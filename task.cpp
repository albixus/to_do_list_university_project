#include "task.h"

std::vector<task> task_vector;

task::task(QDate date,int priority,QString text)
{
    this->date=date;
    this->priority=priority;
    this->text=text;
}
task::task()
{
    date=QDate(2017,1,1);
    priority=0;
    text=QString("");
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

void task::set_check_box(int x,int y,QWidget *parent,int width,int height)
{
    check_box = new QCheckBox(parent);
    check_box->setGeometry(parent->geometry().left()+50+x,y,5000,height);
    check_box->setStyleSheet(" QCheckBox{"
                             "border-radius:5px;"
                            "font-size:12px;"
                            "color:black;}"
                             "QCheckBox:hover{"
                             "background-color: rgba(12, 202, 9,30);}");
    check_box->setMinimumWidth(5000);
    check_box->setCursor(Qt::PointingHandCursor);


    check_box->setText(" "+QString::number(date.day())+"/"+QString::number(date.month())+"/"+QString::number(date.year())+"    "+text);
    check_box->setVisible(true);

    if(priority==1)
        check_box->setStyleSheet("QCheckBox{"
                                 "background-color: rgba(55, 145, 193, 30);"
                                 "border-radius:5px;"
                                 "font-size:12px;}"
                                 "QCheckBox:hover{"
                                 "background-color: rgba(12, 202, 9,30);}"
                                );
    if(priority==2)
        check_box->setStyleSheet("QCheckBox{"
                                 "background-color: rgba(189, 124, 71, 30) ;"
                                 "border-radius:5px;"
                                 "font-size:12px;}"
                                 "QCheckBox:hover{"
                                 "background-color: rgba(12, 202, 9,30);}");
    if(priority==3)
        check_box->setStyleSheet("QCheckBox{"
                                 "background-color: rgba(255, 7, 11, 40);"
                                 "border-radius:5px;"
                                 "font-size:12px;}"
                                 "QCheckBox:hover{"
                                 "background-color: rgba(12, 202, 9,30);}");
}
