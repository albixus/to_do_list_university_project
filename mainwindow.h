#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include "add.h"
#include "task.h"
#include <vector>
#include <QWidget>
#include <QListWidgetItem>
#include "accept.h"
#include "stats.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_burger_button_clicked();

    void on_today_button_clicked();

    void on_week_button_clicked();

    void on_done_button_clicked();

    void on_add_button_clicked();

    void on_task_list_itemChanged(QListWidgetItem *item);

    void on_all_button_clicked();

    void on_overdue_button_clicked();

    void on_clear_button_clicked();

    void on_task_list_customContextMenuRequested(const QPoint &pos);

    void eraseItem();

    void on_stat_button_clicked();

private:
    Ui::MainWindow *ui;
    bool is_burger_button_clicked;
    QPropertyAnimation *sidebar_anim;
    Ui::Add *add_ui;
    void get_today_tasks(std::string filename);
    void save_tasks_from_vectors(std::string filename);
    void get_done_tasks(std::string filename);
    void save_done_tasks(std::string filename);
    void get_next_week_tasks(std::string filename);
    void get_rest_tasks(std::string filename);
    void get_overdue_tasks(std::string filename);
    int check_priority(std::string str);
    void set_color_by_priority(QListWidgetItem *item,int priority);
    void get_points_from_file(std::string filename);
    void save_points_to_file(std::string filename);
    void check_emptiness(const std::vector<std::string> &vec);

    std::vector<QListWidgetItem*> item;
    std::vector<std::string> today;
    std::vector<std::string> next_week;
    std::vector<std::string> rest;
    std::vector<std::string> overdue;

    std::vector<std::string> done;
    enum current_state {TODAY,NEXT_WEEK,REST,OVERDUE,DONE}current;
};

#endif // MAINWINDOW_H
