#ifndef ADD_H
#define ADD_H
#include <QDialog>
#include <fstream>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>

namespace Ui {
class Add;
}

//Adding task
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
    Ui::Add *ui;
    QSqlDatabase db;
};


#endif // ADD_H
