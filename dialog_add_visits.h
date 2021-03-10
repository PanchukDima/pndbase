#ifndef DIALOG_ADD_VISITS_H
#define DIALOG_ADD_VISITS_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
namespace Ui {
class Dialog_add_visits;
}

class Dialog_add_visits : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_visits(QWidget *parent = 0);
    ~Dialog_add_visits();
    int global_param;
    QString global_id;
    QString global_staff;

private:
    Ui::Dialog_add_visits *ui;
public slots:
    void setParam(int,QString,QString);
    void loadData();
    void getData();
    void show_hide_visits_close();
};

#endif // DIALOG_ADD_VISITS_H
