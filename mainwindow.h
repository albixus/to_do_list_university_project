#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

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

private:
    Ui::MainWindow *ui;
    bool is_burger_button_clicked;
    QPropertyAnimation *sidebar_anim;
};

#endif // MAINWINDOW_H
