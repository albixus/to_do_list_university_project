#include "edit_text.h"
#include "ui_edit_text.h"

Edit_text::Edit_text(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit_text)
{
    ui->setupUi(this);
}

Edit_text::~Edit_text()
{
    delete ui;
}

void Edit_text::set_data(QString old_text)
{
    ui->textEdit->setPlainText(old_text);
}

void Edit_text::on_ok_button_clicked()
{
    text = ui->textEdit->toPlainText();
    if(text=="")
    {
        QMessageBox::about(this,"Błąd","Musisz cokolwiek wpisać!");
        return;
    }
    Edit_text::close();
}

void Edit_text::on_cancel_button_clicked()
{
    text = "";
    Edit_text::close();
}
