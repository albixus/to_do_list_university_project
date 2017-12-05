#ifndef ACCEPT_H
#define ACCEPT_H

#include <QDialog>

namespace Ui {
class accept;
}

//dialog box - yes or no
class accept : public QDialog
{
    Q_OBJECT

public:
    explicit accept(QWidget *parent = 0);
    ~accept();

private:
    Ui::accept *ui;
};

#endif // ACCEPT_H
