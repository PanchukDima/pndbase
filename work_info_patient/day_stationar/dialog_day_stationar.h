#ifndef DIALOG_DAY_STATIONAR_H
#define DIALOG_DAY_STATIONAR_H

#include <QDialog>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>
#include "Objects/User/objects_app.h"


namespace Ui {
class Dialog_day_stationar;
}

class Dialog_day_stationar : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_day_stationar(QWidget *parent = 0);
    ~Dialog_day_stationar();
    int global_param;
    QString global_id;

private:
    Ui::Dialog_day_stationar *ui;
public slots:
    void setParam(int,QString);
    void get_sql();
    void load_sql();


};

#endif // DIALOG_DAY_STATIONAR_H
