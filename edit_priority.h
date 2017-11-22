#ifndef EDIT_PRIORITY_H
#define EDIT_PRIORITY_H

#include <QDialog>
#include <QSqlDatabase>
#include <QMessageBox>

namespace Ui {
class Edit_priority;
}

class Edit_priority : public QDialog
{
    Q_OBJECT

public:
    explicit Edit_priority(QWidget *parent = 0);
    int get_priority() { return priority;}
    ~Edit_priority();

private slots:
    void on_ok_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Edit_priority *ui;
    int priority;
};

#endif // EDIT_PRIORITY_H
