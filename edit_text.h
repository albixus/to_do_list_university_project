#ifndef EDIT_TEXT_H
#define EDIT_TEXT_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Edit_text;
}

class Edit_text : public QDialog
{
    Q_OBJECT

public:
    explicit Edit_text(QWidget *parent = 0);
    ~Edit_text();

    void set_data(QString old_text);

    QString get_text()
    {
        return text;
    }

private slots:
    void on_ok_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Edit_text *ui;
    QString text;
};

#endif // EDIT_TEXT_H
