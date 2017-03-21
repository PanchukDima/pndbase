#ifndef DIALOG_DAY_STATIONAR_H
#define DIALOG_DAY_STATIONAR_H

#include <QDialog>

namespace Ui {
class Dialog_day_stationar;
}

class Dialog_day_stationar : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_day_stationar(QWidget *parent = 0);
    ~Dialog_day_stationar();

private:
    Ui::Dialog_day_stationar *ui;
};

#endif // DIALOG_DAY_STATIONAR_H
