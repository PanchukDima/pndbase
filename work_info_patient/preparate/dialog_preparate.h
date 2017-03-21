#ifndef DIALOG_PREPARATE_H
#define DIALOG_PREPARATE_H

#include <QDialog>

namespace Ui {
class Dialog_Preparate;
}

class Dialog_Preparate : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Preparate(QWidget *parent = 0);
    ~Dialog_Preparate();

private:
    Ui::Dialog_Preparate *ui;
};

#endif // DIALOG_PREPARATE_H
