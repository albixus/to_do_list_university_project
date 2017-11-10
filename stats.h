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
    void get_stats_from_file(std::string filename);
    int points;
    int done_tasks;
    int to_do;
};

#endif // STATS_H
