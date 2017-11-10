#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accept.h"

int points;
int task_done_all;

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
    get_rest_tasks("all_tasks.tsk");
    get_overdue_tasks("all_tasks.tsk");
    get_points_from_file("stats.tsk");

    ui->no_task->setHidden(true);
    ui->clear_button->setHidden(true);

    on_today_button_clicked();
    ui->task_list->sortItems(Qt::SortOrder::DescendingOrder);
    current=TODAY;

    ui->rank_label->setText("Twoje Punkty: "+QString::number(points));
    ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
    ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");
    ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
    ui->overdue_button->setText("ZALEGŁE ("+QString::number(overdue.size())+")");

    if(overdue.size()>0)
        QMessageBox::about(this,"Zaległości","Masz zaległe zadania. Sprawdź to!");

    check_emptiness(today);
    is_burger_button_clicked=false;
}

MainWindow::~MainWindow()
{
    save_points_to_file("stats.tsk");
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
        ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
        ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
        ui->stat_button->setText("STATYSTYKI");
        ui->overdue_button->setText("ZALEGŁE ("+QString::number(overdue.size())+")");
        ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");

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

    next_week.clear();
    get_next_week_tasks("all_tasks.tsk");

    today.clear();
    get_today_tasks("all_tasks.tsk");

    rest.clear();
    get_rest_tasks("all_tasks.tsk");

    if(!is_burger_button_clicked)
    {
        ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
        ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");
        ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
    }
}

void MainWindow::on_today_button_clicked()
{
    int priority;
    current=TODAY;
    ui->task_list->clear();
    today.clear();
    item.clear();

    ui->header->setText("DZISIAJ");
    get_today_tasks("all_tasks.tsk");
    ui->clear_button->setHidden(true);

    for(size_t i = 0; i < today.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(today.at(i).c_str())));

        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);

        priority = check_priority(today.at(i));
        set_color_by_priority(item.at(i),priority);
     }
    check_emptiness(today);
}

void MainWindow::on_week_button_clicked()
{
    int priority;
    current=NEXT_WEEK;
    ui->header->setText("NASTĘPNE 7 DNI");
    ui->task_list->clear();
    next_week.clear();
    item.clear();
    get_next_week_tasks("all_tasks.tsk");
    ui->clear_button->setHidden(true);

    for(size_t i = 0; i < next_week.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(next_week.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
        priority = check_priority(next_week.at(i));
        set_color_by_priority(item.at(i),priority);
    }
    check_emptiness(next_week);
}

void MainWindow::on_done_button_clicked()
{
    current=DONE;
    ui->header->setText("WYKONANE ZADANIA");
    ui->task_list->clear();
    done.clear();
    item.clear();
    get_done_tasks("done_tasks.tsk");
    if(done.size()==0)
        ui->clear_button->setHidden(true);
    else
        ui->clear_button->setHidden(false);


    for(size_t i = 0; i < done.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(done.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
    }
    check_emptiness(done);
}

void MainWindow::on_all_button_clicked()
{
    int priority;
    current=REST;
    ui->header->setText("POZOSTAŁE");
    ui->task_list->clear();
    rest.clear();
    item.clear();
    get_rest_tasks("all_tasks.tsk");
    ui->clear_button->setHidden(true);

    for(size_t i = 0; i < rest.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(rest.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
        priority = check_priority(rest.at(i));
        set_color_by_priority(item.at(i),priority);
    }
   check_emptiness(rest);
}

void MainWindow::on_overdue_button_clicked()
{
    int priority;
    current=OVERDUE;
    ui->header->setText("ZALEGŁE");
    ui->task_list->clear();
    overdue.clear();
    item.clear();
    get_overdue_tasks("all_tasks.tsk");
    ui->clear_button->setHidden(true);

    for(size_t i = 0; i < overdue.size();i++)
    {
        item.push_back(new QListWidgetItem(QString::fromUtf8(overdue.at(i).c_str())));
        ui->task_list->addItem(item.at(i));
        item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
        item.at(i)->setCheckState(Qt::Unchecked);
        priority = check_priority(overdue.at(i));
        set_color_by_priority(item.at(i),priority);
    }
    check_emptiness(overdue);
}

int MainWindow::check_priority(std::string str)
{
    return std::atoi(str.substr(11,1).c_str());
}

void MainWindow::set_color_by_priority(QListWidgetItem *item, int priority)
{
    if(priority == 3)
        item->setBackgroundColor(QColor(190, 23, 11,90));
    if(priority == 2)
        item->setBackgroundColor(QColor(255, 177, 42,95));
    if(priority == 1)
        item->setBackgroundColor(QColor(15, 111, 255,80));
}

void MainWindow::check_emptiness(const std::vector<std::string> &vec)
{
    if(vec.size()==0)
        ui->no_task->setHidden(false);
    else
        ui->no_task->setHidden(true);
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
                    check_emptiness(today);
                    points+=30;

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
                    check_emptiness(next_week);
                    points+=50;
                    break;
                }
            for(size_t i= 0; i<rest.size();i++)
                if(text==rest.at(i))
                {
                    done.push_back(rest.at(i));
                    save_done_tasks("done_tasks.tsk");
                    done.clear();
                    get_done_tasks("done_tasks.tsk");
                    rest.erase(rest.begin()+i);
                    save_tasks_from_vectors("all_tasks.tsk");
                    check_emptiness(rest);
                    points+=100;
                    break;
                }
            for(size_t i= 0; i<overdue.size();i++)
                if(text==overdue.at(i))
                {
                    done.push_back(overdue.at(i));
                    save_done_tasks("done_tasks.tsk");
                    done.clear();
                    get_done_tasks("done_tasks.tsk");
                    overdue.erase(rest.begin()+i);
                    save_tasks_from_vectors("all_tasks.tsk");
                    check_emptiness(overdue);
                    points+=10;
                    break;
                }
            task_done_all++;
            ui->rank_label->setText("Twoje Punkty:  "+QString::number(points));
    }
    if(!is_burger_button_clicked)
    {
        ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
        ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");
        ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
        ui->overdue_button->setText("ZALEGŁE ("+QString::number(overdue.size())+")");
    }
}

void MainWindow::get_today_tasks(std::string filename)
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

             if((date.daysTo(task_date)==0))
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

void MainWindow::get_rest_tasks(std::string filename)
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

             if((date.daysTo(task_date)>7))
             {
                 rest.push_back(buffer);
             }
        }

    }
}

void MainWindow::get_overdue_tasks(std::string filename)
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

             if((date.daysTo(task_date)<0))
             {
                 overdue.push_back(buffer);
             }
        }

    }
}

void MainWindow::on_clear_button_clicked()
{
    accept yes_or_no;
    yes_or_no.setModal(false);
    int status = yes_or_no.exec();//exec() return Dialog Code

    if( status == 1)//1 = accepted 0=rejected
    {
        task_done_all = done.size();
        std::fstream file;
        file.open("done_tasks.tsk",std::ios::out |std::ios::trunc);
        file.close();
        on_done_button_clicked();
    }
}

void MainWindow::on_task_list_customContextMenuRequested(const QPoint &pos)
{
    //handle global position
    QPoint global_pos = ui->task_list->mapToGlobal(pos);

    QMenu menu;
    menu.addAction("Usuń",this,SLOT(eraseItem()));

    menu.exec(global_pos);
    eraseItem();
}

void MainWindow::eraseItem()
{
   for(int i=0; i<ui->task_list->selectedItems().size();i++)
    {///ALE MASZ STOWRZONY WEKTOR WSKAZNIKOW NA ITEMY WIEC COS Z TYM TRZEBA ZROBIC A NIE W TAKI SPOSOB JAK PONIZEJ
        //get current item on selected row
        QListWidgetItem *item = ui->task_list->takeItem(ui->task_list->currentRow());
        std::string text = item->text().toStdString();

        for(size_t i= 0; i<today.size();i++)
            if(text==today.at(i))
            {today.erase(today.begin()+i);
                save_tasks_from_vectors("all_tasks.tsk");
                check_emptiness(today);
                break;
            }
        for(size_t i= 0; i<next_week.size();i++)
            if(text==next_week.at(i))
            {
                next_week.erase(next_week.begin()+i);
                save_tasks_from_vectors("all_tasks.tsk");
                check_emptiness(next_week);
                break;
            }
        for(size_t i= 0; i<rest.size();i++)
            if(text==rest.at(i))
            {
                rest.erase(rest.begin()+i);
                save_tasks_from_vectors("all_tasks.tsk");
                check_emptiness(rest);
                break;
            }
        for(size_t i= 0; i<overdue.size();i++)
            if(text==overdue.at(i))
            {
                overdue.erase(rest.begin()+i);
                save_tasks_from_vectors("all_tasks.tsk");
                check_emptiness(overdue);
                break;
            }
        delete item;
        QMessageBox::about(this,"Usunięto","Element został usunięty");
    }
   if(!is_burger_button_clicked)
   {
       ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
       ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");
       ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
       ui->overdue_button->setText("ZALEGŁE ("+QString::number(overdue.size())+")");
   }
}

void MainWindow::get_points_from_file(std::__cxx11::string filename)
{
    std::fstream file;
    std::string buffer;

    file.open(filename,std::ios::in);

    if(file.is_open())
    {
        std::getline(file,buffer);
        points=std::atoi(buffer.c_str());

        std::getline(file,buffer);
        task_done_all=std::atoi(buffer.c_str());
    }
    file.close();
}

void MainWindow::save_points_to_file(std::__cxx11::string filename)
{
    std::fstream file;

    file.open(filename,std::ios::out);

    if(file.is_open())
    {
        file<<std::to_string(points)<<std::endl;
        file<<std::to_string(task_done_all)<<std::endl;
        file<<std::to_string(today.size()+next_week.size()+rest.size()+overdue.size())<<std::endl;
    }
}

void MainWindow::on_stat_button_clicked()
{
    save_points_to_file("stats.tsk");
    Stats *stat = new Stats;
    stat->show();
}
