#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sidebar_anim = new QPropertyAnimation(ui->sidebar,"minimumSize");
    sidebar_anim->setDuration(500);

   // QCheckBox *a = new QCheckBox(ui->task_area_content);

  //  a->setLayout(ui->tasks->layout());
  //  a->setGeometry(ui->task_area->geometry().top()+50,ui->task_area->geometry().left()+70,1000,30);
  //  a->setText(QString("CHECKBOX Z KODU"));

    is_burger_button_clicked=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_burger_button_clicked()
{
    if(is_burger_button_clicked)
    {
        sidebar_anim->setStartValue(ui->sidebar->minimumSize());
        sidebar_anim->setEndValue(QSize(200,0));
        is_burger_button_clicked=false;
    }
    else
    {
        sidebar_anim->setStartValue(ui->sidebar->minimumSize());
        sidebar_anim->setEndValue(QSize(100,0));
        is_burger_button_clicked=true;
    }
        sidebar_anim->start();
}

void MainWindow::on_add_button_clicked()
{
    Add add_dialog;
    add_dialog.setModal(false);
    add_dialog.exec();
    task_vector[task_vector.size()-1].set_check_box(0,ui->task_area->geometry().left()+70,ui->task_area_content);
}

void MainWindow::on_today_button_clicked()
{
    ui->header->setText("DZISIAJ");
}

void MainWindow::on_week_button_clicked()
{
    ui->header->setText("NASTĘPNE 7 DNI");
}

void MainWindow::on_done_button_clicked()
{
    ui->header->setText("WYKONANE ZADANIA");
}


