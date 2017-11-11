#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QCheckBox>
#include <QDate>
#include <QMessageBox>
#include <QMenu>
#include <QObject>


class task
{
public:
    explicit task();

    void set_date(QDate date);
    void set_priority(int priority);
    void set_text(QString text);
    QString get_complete_text() { return "Priorytet: "+QString::number(priority)+" "+QString::number(date.day())+"/"+QString::number(date.month())+"/"+QString::number(date.year())+"   "+text;}
    int get_priority() { return priority;}

private:
    QDate date;
    int priority;
    QString text;
};



#endif // TASK_H
