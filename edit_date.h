#ifndef EDIT_DATE_H
#define EDIT_DATE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class edit_date;
}

//edit date from context menu
class edit_date : public QDialog
{
    Q_OBJECT

public:
    explicit edit_date(QWidget *parent = 0);
    ~edit_date();

    QDate get_date()
    {
        return active_date;
    }

private slots:
    void on_cancel_button_clicked();

    void on_ok_button_clicked();

private:
    Ui::edit_date *ui;
    QDate active_date;
};

#endif // EDIT_DATE_H
