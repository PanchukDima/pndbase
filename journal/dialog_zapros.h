#ifndef DIALOG_ZAPROS_H
#define DIALOG_ZAPROS_H

#include <QDialog>

namespace Ui {
class Dialog_zapros;
}

class Dialog_zapros : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_zapros(QWidget *parent = 0);
    ~Dialog_zapros();

private:
    Ui::Dialog_zapros *ui;
};

#endif // DIALOG_ZAPROS_H
