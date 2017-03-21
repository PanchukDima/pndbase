#ifndef DIALOG_TABLE_ADD_H
#define DIALOG_TABLE_ADD_H

#include <QDialog>

namespace Ui {
class Dialog_table_add;
}

class Dialog_table_add : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_table_add(QWidget *parent = 0);
    ~Dialog_table_add();
    int x;
    int y;
signals:
    void table(int,int);
private:
    Ui::Dialog_table_add *ui;
public slots:
    void create_table();
};

#endif // DIALOG_TABLE_ADD_H
