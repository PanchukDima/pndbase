#ifndef DIALOG_LIST_NOT_WORK_H
#define DIALOG_LIST_NOT_WORK_H

#include <QDialog>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>
#include "Objects/User/objects_app.h"

namespace Ui {
class Dialog_list_not_work;
}

class Dialog_list_not_work : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_list_not_work(QWidget *parent = 0);
    ~Dialog_list_not_work();
    QString global_id;
    QString global_staff_id;
    int global_param;

private:
    Ui::Dialog_list_not_work *ui;
public slots:
    void setParam(int, QString );
    void load_data();
    void send_data();
    void load_lists();
    void date_close_edit(QDate);
    void show_or_hide_close(bool);
    void changes_code_diagnos(int);
    void changes_name_diagnos(int);
    void load_current_diagnos();
};

#endif // DIALOG_LIST_NOT_WORK_H
