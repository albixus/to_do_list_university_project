#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QListWidgetItem>
#include <QKeyEvent>

#include <vector>

#include "add.h"
#include "task.h"
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

protected:
    void keyPressEvent(QKeyEvent *key);

private:
    Ui::MainWindow *ui;
    Ui::Add *add_ui;
    QPropertyAnimation *sidebar_anim;

    //vectors for diffrent tasks
    std::vector<QListWidgetItem*> item;
    std::vector<std::string> today;
    std::vector<std::string> next_week;
    std::vector<std::string> rest;
    std::vector<std::string> overdue;
    std::vector<std::string> done;

    enum current_state {TODAY,NEXT_WEEK,REST,OVERDUE,DONE}current;

    bool is_burger_button_clicked;

    ///getting diffrent tasks
    void get_today_tasks(std::string filename);  
    void get_done_tasks(std::string filename);
    void get_next_week_tasks(std::string filename);
    void get_rest_tasks(std::string filename);
    void get_overdue_tasks(std::string filename);

    ///writing and reading stats from file
    void get_points_from_file(std::string filename);
    void save_points_to_file(std::string filename);

    ///saving tasks to file
    void save_done_tasks(std::string filename);
    void save_tasks_from_vectors(std::string filename);

    int check_priority(std::string str);
    void set_color_by_priority(QListWidgetItem *item,int priority);
    void check_emptiness(const std::vector<std::string> &vec);  
};

#endif // MAINWINDOW_H
