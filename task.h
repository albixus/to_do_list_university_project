#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QCheckBox>
#include <QDate>
#include <QMessageBox>


class task
{
public:
    task(QDate date,int priority,QString text);
    task();

    void set_date(QDate date);
    void set_priority(int priority);
    void set_text(QString text);
    void set_check_box(int x,int y,QWidget *parent=nullptr,int width=2000,int height=30);
    QString get_text() { return text;}
private:
    QDate date;
    int priority;
    QString text;
    QCheckBox *check_box;
};

extern std::vector<task> task_vector;

#endif // TASK_H
