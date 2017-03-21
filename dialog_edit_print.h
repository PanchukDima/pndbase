#ifndef DIALOG_EDIT_PRINT_H
#define DIALOG_EDIT_PRINT_H

#include <QDialog>

namespace Ui {
class Dialog_edit_print;
}

class Dialog_edit_print : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_edit_print(QWidget *parent = 0);
    ~Dialog_edit_print();

private:
    Ui::Dialog_edit_print *ui;
public slots:

};

#endif // DIALOG_EDIT_PRINT_H
