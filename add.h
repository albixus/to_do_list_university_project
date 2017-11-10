#ifndef ADD_H
#define ADD_H
#include <QDialog>
#include "task.h"
#include <fstream>

namespace Ui {
class Add;
}

class Add : public QDialog
{
    Q_OBJECT

public:
    explicit Add(QWidget *parent = 0);
    ~Add();

private slots:
    void on_ok_button_clicked();

    void on_cancel_button_clicked();

    void on_calendar_activated(const QDate &date);

private:
    std::fstream file;

    Ui::Add *ui;
    task tmp_task;
    void write_to_file(std::string filename);
};


#endif // ADD_H
