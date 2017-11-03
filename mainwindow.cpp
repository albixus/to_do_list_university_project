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



    is_burger_button_clicked=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_burger_button_clicked()
{
    ui->add_button->setIcon(QIcon(":/images/img/plus.png"));
    ui->done_button->setIcon(QIcon(":/images/img/checkmark.png"));
    ui->week_button->setIcon(QIcon(":/images/img/7days.png"));
    ui->today_button->setIcon(QIcon(":/images/img/today.png"));

    if(is_burger_button_clicked)
    {
        sidebar_anim->setStartValue(ui->sidebar->minimumSize());
        sidebar_anim->setEndValue(QSize(200,0));
        ui->add_button->setText("DODAJ");
        ui->done_button->setText("DZISIAJ");
        ui->week_button->setText("NASTEPNE 7 DNI");
        ui->today_button->setText("WYKONANE");

        ui->add_button->setIconSize(QSize(0,0));
        ui->done_button->setIconSize(QSize(0,0));
        ui->week_button->setIconSize(QSize(0,0));
        ui->today_button->setIconSize(QSize(0,0));
        is_burger_button_clicked=false;
    }
    else
    {
        sidebar_anim->setStartValue(ui->sidebar->minimumSize());
        sidebar_anim->setEndValue(QSize(100,0));

        ui->add_button->setText("");
        ui->done_button->setText("");
        ui->week_button->setText("");
        ui->today_button->setText("");

        ui->add_button->setIconSize(QSize(30,30));
        ui->done_button->setIconSize(QSize(30,30));
        ui->week_button->setIconSize(QSize(30,30));
        ui->today_button->setIconSize(QSize(30,30));

        is_burger_button_clicked=true;
    }
        sidebar_anim->start();
}

void MainWindow::on_add_button_clicked()
{
    Add add_dialog;
    add_dialog.setModal(false);
    add_dialog.exec();

    size_t size = task_vector.size();

    if(size!=0)
        task_vector[size-1].set_check_box(0,ui->task_area->geometry().top()+20+(40*size),ui->task_area_content);

    if(size>11)
        ui->task_area_content->setFixedSize(ui->task_area_content->width(),ui->task_area_content->height()+40);
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


