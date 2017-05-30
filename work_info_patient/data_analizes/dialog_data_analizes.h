#ifndef DIALOG_DATA_ANALIZES_H
#define DIALOG_DATA_ANALIZES_H

#include <QDialog>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>
#include "Objects/User/objects_app.h"

namespace Ui {
class Dialog_data_analizes;
}

class Dialog_data_analizes : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_data_analizes(QWidget *parent = 0);
    ~Dialog_data_analizes();
    QString global_id;
    int global_param;

private:
    Ui::Dialog_data_analizes *ui;
public slots:
    void load_library();
    void load_sql();
    void get_sql();
    void setParam(int,QString);
};

#endif // DIALOG_DATA_ANALIZES_H
