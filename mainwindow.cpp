#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sidebar_anim = new QPropertyAnimation(ui->sidebar,"minimumSize");
    sidebar_anim->setDuration(500);
    get_today_tasks("all_tasks.tsk");



    for(size_t i = 0; i < today.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(today.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
    }


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
    ui->stat_button->setIcon(QIcon(":/images/img/statistics.png"));
    ui->overdue_button->setIcon(QIcon(":/images/img/past.png"));

    if(is_burger_button_clicked)
    {
        sidebar_anim->setStartValue(ui->sidebar->minimumSize());
        sidebar_anim->setEndValue(QSize(200,0));
        ui->add_button->setText("DODAJ");
        ui->done_button->setText("WYKONANE");
        ui->week_button->setText("NASTEPNE 7 DNI");
        ui->today_button->setText("DZISIAJ");
        ui->stat_button->setText("STATYSTYKI");
        ui->overdue_button->setText("ZALEGŁE");

        ui->add_button->setIconSize(QSize(0,0));
        ui->done_button->setIconSize(QSize(0,0));
        ui->week_button->setIconSize(QSize(0,0));
        ui->today_button->setIconSize(QSize(0,0));
        ui->stat_button->setIconSize(QSize(0,0));
        ui->overdue_button->setIconSize(QSize(0,0));

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
        ui->stat_button->setText("");
        ui->overdue_button->setText("");

        ui->add_button->setIconSize(QSize(30,30));
        ui->done_button->setIconSize(QSize(30,30));
        ui->week_button->setIconSize(QSize(30,30));
        ui->today_button->setIconSize(QSize(30,30));
        ui->stat_button->setIconSize(QSize(30,30));
        ui->overdue_button->setIconSize(QSize(30,30));

        is_burger_button_clicked=true;
    }
        sidebar_anim->start();
}

void MainWindow::on_add_button_clicked()
{
    Add add_dialog;
    add_dialog.setModal(false);
    add_dialog.exec();
}

void MainWindow::on_today_button_clicked()
{
    ui->task_list->clear();
    today.clear();
    item.clear();

    ui->header->setText("DZISIAJ");
    get_today_tasks("all_tasks.tsk");

    for(size_t i = 0; i < today.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(today.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::on_week_button_clicked()
{
    ui->header->setText("NASTĘPNE 7 DNI");
}

void MainWindow::on_done_button_clicked()
{
    ui->header->setText("WYKONANE ZADANIA");
}

void MainWindow::on_task_list_itemChanged(QListWidgetItem *item)
{
    if(item->checkState())
    {
            item->setHidden(true);
    }
}

void MainWindow::get_today_tasks(std::string filename)
{
    std::fstream file;
    QDate date = QDate::currentDate();
    std::string string_date= QString(QString::number(date.day())+"/"+QString::number(date.month())+"/"+QString::number(date.year())).toStdString();
    std::string buffer;
    std::string date_buffer;

    file.open(filename,std::ios::in);
    if(file.is_open())
    {
        while(!file.eof())
        {
            std::getline(file,buffer);
            if(buffer=="")
                break;
            date_buffer=buffer.substr(13,buffer.find_first_of(" ")-1);//get only date from file then compare it with current date if they are the same add to vector
            if(date_buffer==string_date)
            {
                today.push_back(buffer);
            }
        }
    }
    else
        QMessageBox::about(this,"ERROR","Cannot write to file");
}
