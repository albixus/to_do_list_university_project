#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "accept.h"
#include <QSettings>

int points;
int task_done_all;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize_tray();
    sidebar_anim = new QPropertyAnimation(ui->sidebar,"minimumSize");
    sidebar_anim->setDuration(500);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath()+"/to_do_list.sqlite");
    if(db.open())
    {
        QSqlQuery query;
        query.exec("CREATE TABLE \"tasks\" (\"ID_Task\" INTEGER PRIMARY KEY  NOT NULL ,\"Priority\" INTEGER NOT NULL ,"
                       "\"Data\" DATETIME NOT NULL  DEFAULT (null) ,\"Text\" VARCHAR(100) NOT NULL ,\"Is_Done\" BOOL NOT NULL  DEFAULT (0))");
    }

    get_points_from_file("stats.tsk");

    if(overdue.size()>0)
        QMessageBox::about(this,"Zaległości","Masz zaległe zadania. Sprawdź to!");

    ui->no_task->setHidden(true);
    ui->clear_button->setHidden(true);
    is_burger_button_clicked=false;

    on_today_button_clicked();
    ui->task_list->sortItems(Qt::SortOrder::DescendingOrder);

    get_tasks(NEXT_WEEK);
    get_tasks(OVERDUE);
    get_tasks(REST);

    ui->rank_label->setText("Twoje Punkty: "+QString::number(points));

    set_number_in_sidebar();
}

MainWindow::~MainWindow()
{
    save_points_to_file("stats.tsk");
    db.close();
    QSqlDatabase::removeDatabase(qApp->applicationDirPath()+"/to_do_list.sqlite");
    delete ui;
}

//SLOTS SECTION/////////////////////////////////////////////////////////

void MainWindow::on_show()
{
    this->showNormal();
    this->setWindowState(Qt::WindowActive);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::DoubleClick:
        on_show();
        break;
    default:
        break;
    }
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
    db.close();

    QSqlDatabase::removeDatabase(qApp->applicationDirPath()+"/to_do_list.sqlite");
    Add add_dialog;
    add_dialog.setModal(false);
    add_dialog.exec();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(qApp->applicationDirPath()+"/to_do_list.sqlite");

    db.open();

    current_state tmp=current;
        on_today_button_clicked();
        on_week_button_clicked();
        on_all_button_clicked();
        on_overdue_button_clicked();
        on_done_button_clicked();
    current=tmp;

    switch(current)
    {
    case TODAY:
        on_today_button_clicked();
        break;
    case NEXT_WEEK:
        on_week_button_clicked();
        break;
    case REST:
        on_all_button_clicked();
        break;
    case OVERDUE:
        on_overdue_button_clicked();
        break;
    case DONE:
        on_done_button_clicked();
        break;
    }

    set_number_in_sidebar();
}

void MainWindow::on_today_button_clicked()
{
    current=TODAY;
    ui->header->setText("DZISIAJ");
    ui->clear_button->setHidden(true);

    set_items(today);
    set_number_in_sidebar();

}

void MainWindow::on_week_button_clicked()
{
    current=NEXT_WEEK;
    ui->header->setText("NASTĘPNE 7 DNI");
    ui->clear_button->setHidden(true);
    set_items(next_week);
}

void MainWindow::on_done_button_clicked()
{
    current=DONE;
    ui->header->setText("WYKONANE ZADANIA");

    set_items(done);

    if(done.size()==0)
        ui->clear_button->setHidden(true);
    else
        ui->clear_button->setHidden(false);
}

void MainWindow::on_all_button_clicked()
{
    current=REST;
    ui->header->setText("POZOSTAŁE");
    ui->clear_button->setHidden(true);
    set_items(rest);
}

void MainWindow::on_overdue_button_clicked()
{
    current=OVERDUE;
    ui->header->setText("ZALEGŁE");
    ui->clear_button->setHidden(true);

    set_items(overdue);
}

void MainWindow::on_task_list_itemChanged(QListWidgetItem *item)
{
    if(item->checkState())
    {
            item->setHidden(true);
                QSqlQuery query;
                if(!query.exec("UPDATE tasks SET Is_Done = 1 WHERE ID_Task = "+item->data(Qt::UserRole).toString()+";"))
                {
                    QMessageBox::about(this,"Błąd","Błąd zapytania do bazy");
                }
            task_done_all++;
            update_vectors(item->text().toStdString(),true);
    }
}

void MainWindow::on_clear_button_clicked()
{
    accept yes_or_no;
    yes_or_no.setModal(false);
    int status = yes_or_no.exec();//exec() return Dialog Code

    if( status == 1)//1 = accepted 0=rejected
    {
       QSqlQuery query;
       if(!query.exec("DELETE  FROM tasks WHERE Is_Done = 1;"))
            QMessageBox::about(this,"Błąd","Błąd zapytania do bazy!");

       on_done_button_clicked();
    }
}

void MainWindow::on_task_list_customContextMenuRequested(const QPoint &pos)
{
    //handle global position
    if(current!=DONE)
    {
        QPoint global_pos = ui->task_list->mapToGlobal(pos);
        QAction erase;

        QMenu menu;
        menu.addAction("Usuń",this,SLOT(eraseItem()));
        menu.addSeparator();
        menu.addAction("Edytuj Priorytet",this,SLOT(on_priority_edit()));
        menu.addAction("Edytuj Date",this,SLOT(on_date_edit()));
        menu.addAction("Edytuj Treść",this,SLOT(on_text_edit()));

        menu.exec(global_pos);

        current_state tmp_current = current;
        on_today_button_clicked();
        on_week_button_clicked();
        on_all_button_clicked();

        switch(tmp_current)
        {
        case TODAY:
            on_today_button_clicked();
            break;
        case NEXT_WEEK:
            on_week_button_clicked();
            break;
        case REST:
            on_all_button_clicked();
            break;
        case OVERDUE:
            break;
        case DONE:
            break;
        }
    }
}

void MainWindow::eraseItem()
{
   for(int i=0; i<ui->task_list->selectedItems().size();i++)
   {

        //get current item on selected row
        QListWidgetItem *item = ui->task_list->takeItem(ui->task_list->currentRow());

        QSqlQuery query;
        if(!query.exec("DELETE FROM tasks WHERE ID_Task = "+item->data(Qt::UserRole).toString()+";"))
        {
            QMessageBox::about(this,"Błąd","Błąd zapytania do bazy");
        }

        update_vectors(item->text().toStdString(),false);
   }
}

void MainWindow::on_stat_button_clicked()
{
    save_points_to_file("stats.tsk");
    Stats *stat = new Stats;
    stat->show();
}

void MainWindow::on_priority_edit()
{
    for(int i=0; i<ui->task_list->selectedItems().size();i++)
     {
        //get current item on selected row
        QListWidgetItem *item = ui->task_list->item(ui->task_list->currentRow());

        Edit_priority dialog;
        dialog.setModal(false);
        dialog.exec();

            int priority = dialog.get_priority();
            if(priority != -1)
            {
                QSqlQuery query;
                if(!query.exec("UPDATE tasks SET Priority ="+QString::number(priority)+" WHERE ID_Task ="+item->data(Qt::UserRole).toString()+";"))
                    QMessageBox::about(this,"Błąd","Błąd zapytania do bazy priority edit");
            }

    }
}

void MainWindow::on_date_edit()
{
    for(int i=0; i<ui->task_list->selectedItems().size();i++)
     {
        //get current item on selected row
        QListWidgetItem *item = ui->task_list->item(ui->task_list->currentRow());

       edit_date dialog;
       dialog.setModal(false);
       dialog.exec();

       QDate edited_date = dialog.get_date();

       if(edited_date.year()!=1000)
       {
            QSqlQuery query;
            if(!query.exec("UPDATE tasks SET Data = '"+ edited_date.toString("yyyy-MM-dd")+
                           "' WHERE ID_Task = "+item->data(Qt::UserRole).toString()+";"))
                QMessageBox::about(this,"Błąd","Błąd zapytania do bazy date edit");
       }

       update_vectors(item->text().toStdString().c_str(),false);
     }
}

void MainWindow::on_text_edit()
{
    for(int i=0; i<ui->task_list->selectedItems().size();i++)
     {
        //get current item on selected row
        QListWidgetItem *item = ui->task_list->item(ui->task_list->currentRow());

        Edit_text dialog;
        dialog.setModal(false);
        std::string tmp_text = item->text().toStdString();
        tmp_text.erase(0,25);
        dialog.set_data(QString::fromUtf8(tmp_text.c_str()));
        dialog.exec();

        QString text = dialog.get_text();

        if(text!="")
        {
            QSqlQuery query;
            if(!query.exec("UPDATE tasks SET text = '" + text + "' WHERE ID_Task = "+item->data(Qt::UserRole).toString()+";"))
                QMessageBox::about(this,"Błąd","Błąd zapytania do bazy date edit");
        }
    }
}

//READING FROM FILES AND DATABASE SECTION/////////////////////////////////////////////

void MainWindow::get_tasks(enum current_state current)
{
        QSqlQuery query;
        if(current == TODAY)
        {
            if(query.exec("SELECT *  FROM tasks WHERE data = date('now');"))
            {
                while(query.next())
                {
                    if(!query.value(4).toBool())
                         today.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                    else
                         done.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                }
            }
            else
                 QMessageBox::about(this,"Error","Błąd zapytania do bazy TODAY!");
        }
        if(current == NEXT_WEEK)
        {
            if(query.exec("SELECT *  FROM tasks WHERE data BETWEEN date('now','1 day') AND date('now','7 days');"))
            {
                while(query.next())
                {
                    if(!query.value(4).toBool())
                       next_week.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                    else
                        done.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                }
            }
            else
                 QMessageBox::about(this,"Error","Błąd zapytania do bazy!");
        }
        if(current == OVERDUE)
        {
            if(query.exec("SELECT *  FROM tasks WHERE date(data) < date('now');"))
            {
                while(query.next())
                {
                    if(!query.value(4).toBool())
                       overdue.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                    else
                       done.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                }
            }
            else
                 QMessageBox::about(this,"Error","Błąd zapytania do bazy!");
        }
        if(current == REST)
        {
            if(query.exec("SELECT *  FROM tasks WHERE data BETWEEN date('now','8 day') AND date('now','2000 days');"))
            {
                while(query.next())
                    if(!query.value(4).toBool())
                       rest.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
                    else
                       done.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
            }
            else
                 QMessageBox::about(this,"Error","Błąd zapytania do bazy!");
        }
        if(current == DONE)
        {
            if(query.exec("SELECT * FROM tasks WHERE Is_Done = 1;"))
                while(query.next())
                    done.push_back((query.value(0).toString()+" "+query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString()).toStdString());
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

//SAVING TO FILES SECTION///////////////////////////////////////////////

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

//OTHER METHODS/////////////////////////////////////////////////////////
int MainWindow::check_priority(std::string str)
{
    return std::atoi(str.substr(10,2).c_str());
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

void MainWindow::update_vectors(std::string text, bool with_points)
{
    text.erase(0,11);

    for(size_t i= 0; i<today.size();i++)
    {
        if(text==today.at(i))
        {
            today.erase(today.begin()+i);
            check_emptiness(today);
            if(with_points)
                points+=20;
            break;
        }
    }
    for(size_t i= 0; i<next_week.size();i++)
        if(text==next_week.at(i))
        {
            next_week.erase(next_week.begin()+i);
            check_emptiness(next_week);
            if(with_points)
                points+=50;
            break;
        }
    for(size_t i= 0; i<rest.size();i++)
        if(text==rest.at(i))
        {
            rest.erase(rest.begin()+i);
            check_emptiness(rest);
            if(with_points)
                points+=100;
            break;
        }
    for(size_t i= 0; i<overdue.size();i++)
        if(text==overdue.at(i))
        {
            overdue.erase(overdue.begin()+i);
            check_emptiness(overdue);
            if(with_points)
                points+=10;
            break;
        }

    ui->rank_label->setText("Twoje Punkty: "+QString::number(points));

    set_number_in_sidebar();
}

void MainWindow::keyPressEvent(QKeyEvent *key)
{
    if(key->key() == Qt::Key_Return)
        on_add_button_clicked();
}

void MainWindow::initialize_tray()
{
    //setting tray icon
    if(QSystemTrayIcon::isSystemTrayAvailable() == false)
        QMessageBox::critical(this,"Błąd","Tray icon nie jest dostępny na tym komputerze");

    minimizeAction = new QAction(tr("Mi&nimalizuj"),this);
    connect(minimizeAction, SIGNAL(triggered(bool)),this,SLOT(showMinimized()));

    maximizeAction = new QAction(tr("Ma&ksymalizuj"),this);
    connect(maximizeAction,SIGNAL(triggered(bool)),this,SLOT(showMaximized()));

    showAction = new QAction(tr("&Wyświetl"),this);
    connect(showAction,SIGNAL(triggered(bool)),this,SLOT(on_show()));

    quitAction = new QAction(tr("&Zakończ"),this);
    connect(quitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    //adding actions to tray context menu
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    //setting tray on our mainwindow
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    //connecting events
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //setting icon
    trayIcon->setIcon(this->windowIcon());
    trayIcon->show();

}

void MainWindow::set_items(std::vector<std::string> &vec)
{
    ui->task_list->clear();
    vec.clear();
    item.clear();
    get_tasks(current);

    for(size_t i = 0; i < vec.size();i++)
    {
        std::string item_text = vec.at(i);
        int id = std::atoi(item_text.substr(0,2).c_str());
        item_text.erase(0,2);

        item_text = "Priorytet: " + item_text;
        QVariant id_data(id);
        item.push_back(new QListWidgetItem(QString::fromUtf8(item_text.c_str())));

        //setting primary key as data to every ListWidgetItem
        item.at(i)->setData(Qt::UserRole,id_data);

        ui->task_list->addItem(item.at(i));
        if(current!=DONE)
        {
            item.at(i)->setFlags(item.at(i)->flags()|Qt::ItemIsUserCheckable);
            item.at(i)->setCheckState(Qt::Unchecked);
        }

        vec.at(i).erase(0,2);

        int priority = check_priority(item_text);
        set_color_by_priority(item.at(i),priority);
     }

    check_emptiness(vec);
    set_number_in_sidebar();
}

void MainWindow::changeEvent(QEvent* e)
{
    switch (e->type())
    {
        case QEvent::WindowStateChange:
            {
                if (this->windowState() & Qt::WindowMinimized)
                {
                    this->hide();
                }
                break;
            }
        default:
            break;
    }

    QMainWindow::changeEvent(e);
}

void MainWindow::set_number_in_sidebar()
{
    if(!is_burger_button_clicked)
    {
        ui->today_button->setText("DZISIAJ ("+QString::number(today.size())+")");
        ui->all_button->setText("POZOSTAŁE ("+QString::number(rest.size())+")");
        ui->week_button->setText("NASTĘPNY TYDZIEŃ ("+QString::number(next_week.size())+")");
        ui->overdue_button->setText("ZALEGŁE ("+QString::number(overdue.size())+")");
    }
}
