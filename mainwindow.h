#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include "add.h"
#include "task.h"
#include <vector>
#include <QWidget>
#include <QListWidgetItem>

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

private:
    Ui::MainWindow *ui;
    bool is_burger_button_clicked;
    QPropertyAnimation *sidebar_anim;
    Ui::Add *add_ui;
    std::vector<QListWidgetItem*> list_items;
    void get_today_tasks(std::string filename);

    std::vector<QListWidgetItem*> item;

    std::vector<std::string> today;
    std::vector<std::string> next_week;
    std::vector<std::string> done;
    std::vector<std::string> overdue;
};

#endif // MAINWINDOW_H
