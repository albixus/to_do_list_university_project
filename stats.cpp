#include "stats.h"
#include "ui_stats.h"
#include <QMessageBox>

Stats::Stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stats)
{
    ui->setupUi(this);
    get_stats_from_file("stats.tsk");
    ui->points_lcd->display(points);
    ui->done_lcd->display(done_tasks);
    ui->to_do_lcd->display(to_do);

    if(points<5000)
    {
        ui->rank_label->setText("TWOJA RANGA: AMATOR");
        ui->rank_icon->setIcon(QIcon(":/images/img/medal3.png"));
    }
    else if((points>=5000)&&(points<10000))
    {
        ui->rank_label->setText("TWOJA RANGA: ZAAWANSOWANY");
        ui->rank_icon->setIcon(QIcon(":/images/img/medal2.png"));
    }
    else if((points>=10000)&&(points<20000))
    {
        ui->rank_label->setText("TWOJA RANGA: PROFESJONALISTA");
        ui->rank_icon->setIcon(QIcon(":/images/img/medal1.png"));
    }
    else if(points>=20000)
    {
        ui->rank_label->setText("TWOJA RANGA: MISTRZ PRODUKTYWNOŚCI");
        ui->rank_icon->setIcon(QIcon(":/images/img/best_medal.png"));
    }
}

Stats::~Stats()
{
    delete ui;
}

void Stats::get_stats_from_file(std::__cxx11::string filename)
{
    std::fstream file;
    std::string buffer;

    file.open(filename,std::ios::in);

    if(file.is_open())
    {
        std::getline(file,buffer);
        points = std::atoi(buffer.c_str());

        std::getline(file,buffer);
        done_tasks = std::atoi(buffer.c_str());

        std::getline(file,buffer);
        to_do = std::atoi(buffer.c_str());
    }

    file.close();
}
