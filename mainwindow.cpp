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
    get_done_tasks("done_tasks.tsk");
    get_next_week_tasks("all_tasks.tsk");


    for(size_t i = 0; i < today.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(today.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
    }
    ui->task_list->sortItems(Qt::SortOrder::DescendingOrder);


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
    ui->all_button->setIcon(QIcon(":/images/img/all.png"));

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
        ui->all_button->setText("WSZYSTKIE");

        ui->add_button->setIconSize(QSize(0,0));
        ui->done_button->setIconSize(QSize(0,0));
        ui->week_button->setIconSize(QSize(0,0));
        ui->today_button->setIconSize(QSize(0,0));
        ui->stat_button->setIconSize(QSize(0,0));
        ui->overdue_button->setIconSize(QSize(0,0));
        ui->all_button->setIconSize(QSize(0,0));

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
        ui->all_button->setText("");

        ui->add_button->setIconSize(QSize(30,30));
        ui->done_button->setIconSize(QSize(30,30));
        ui->week_button->setIconSize(QSize(30,30));
        ui->today_button->setIconSize(QSize(30,30));
        ui->stat_button->setIconSize(QSize(30,30));
        ui->overdue_button->setIconSize(QSize(30,30));
        ui->all_button->setIconSize(QSize(35,35));

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
    ui->task_list->clear();
    next_week.clear();
    item.clear();
    get_next_week_tasks("all_tasks.tsk");

    for(size_t i = 0; i < next_week.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(next_week.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::on_done_button_clicked()
{
    ui->header->setText("WYKONANE ZADANIA");
    ui->task_list->clear();
    item.clear();

    for(size_t i = 0; i < done.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(done.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
    }
}

void MainWindow::on_task_list_itemChanged(QListWidgetItem *item)
{
    if(item->checkState())
    {
            item->setHidden(true);
            std::string text = item->text().toStdString();

            for(size_t i= 0; i<today.size();i++)
                if(text==today.at(i))
                {
                    done.push_back(today.at(i));
                    save_done_tasks("done_tasks.tsk");
                    done.clear();
                    get_done_tasks("done_tasks.tsk");
                    today.erase(today.begin()+i);
                    save_tasks_from_vectors("all_tasks.tsk");

                    break;
                }
            for(size_t i= 0; i<next_week.size();i++)
                if(text==next_week.at(i))
                {
                    done.push_back(next_week.at(i));
                    save_done_tasks("done_tasks.tsk");
                    done.clear();
                    get_done_tasks("done_tasks.tsk");
                    next_week.erase(next_week.begin()+i);
                    save_tasks_from_vectors("all_tasks.tsk");
                    break;
                }

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
        QMessageBox::about(this,"ERROR","Cannot open file");

    file.close();
}

void MainWindow::save_tasks_from_vectors(std::string filename)
{
    std::fstream file;

    file.open(filename,std::ios::out);

    if(file.is_open())
    {
        size_t i;

        for(i=0;i<today.size();i++)
            file<<today.at(i)<<std::endl;
        for(i=0;i<next_week.size();i++)
            file<<next_week.at(i)<<std::endl;
        for(i=0;i<rest.size();i++)
            file<<rest.at(i)<<std::endl;
        for(i=0;i<overdue.size();i++)
            file<<overdue.at(i)<<std::endl;
    }
    else
        QMessageBox::about(this,"ERROR","Cannot open file");
    file.close();
}

void MainWindow::get_done_tasks(std::string filename)
{
    std::fstream file;
    file.open(filename,std::ios::in);
    std::string buffer;

    if(file.is_open())
    {
        while(!file.eof())
        {
            std::getline(file,buffer);

            if(buffer=="")
                break;

            done.push_back(buffer);
        }
    }

    file.close();
}

void MainWindow::save_done_tasks(std::string filename)
{
    std::fstream file;

    file.open(filename,std::ios::out);

    if(file.is_open())
    {
        for(size_t i=0;i<done.size();i++)
            file<<done.at(i)<<std::endl;
    }
    else
        QMessageBox::about(this,"ERROR","Cannot open file");
    file.close();
}

void MainWindow::get_next_week_tasks(std::string filename)
{
    std::fstream file;
    QDate date = QDate::currentDate();
    std::string date_buffer;
    std::string buffer;
    int day,month,year;
    size_t pos;

    file.open(filename,std::ios::in);
    if(file.is_open())
    {
        while(!file.eof())
        {
            std::getline(file,buffer);
            if(buffer=="")
                break;
             date_buffer=buffer.substr(13,buffer.find_first_of(" "));

             pos=date_buffer.find_first_of("/");
             day=std::atoi(date_buffer.substr(0,pos).c_str());

             date_buffer.erase(0,pos+1);
             pos=date_buffer.find_first_of("/");

             month=std::atoi(date_buffer.substr(0,pos).c_str());
             date_buffer.erase(0,pos+1);

             year=std::atoi(date_buffer.c_str());

             QDate task_date(year,month,day);

             if((date.daysTo(task_date)<=7)&&(date.daysTo(task_date)>0))
             {
                 next_week.push_back(buffer);
             }
        }
    }
    else
        QMessageBox::about(this,"ERROR","Cannot open file");

    file.close();
}
