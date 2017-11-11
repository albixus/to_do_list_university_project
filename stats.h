#ifndef STATS_H
#define STATS_H

#include <QWidget>
#include <fstream>

namespace Ui {
class Stats;
}

class Stats : public QWidget
{
    Q_OBJECT

public:
    explicit Stats(QWidget *parent = 0);
    ~Stats();

private:
    Ui::Stats *ui;

    int points;
    int done_tasks;
    int to_do;

    void get_stats_from_file(std::string filename);
};

#endif // STATS_H
