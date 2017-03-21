#ifndef DIALOG_VISITING_CONTROL_H
#define DIALOG_VISITING_CONTROL_H

#include <QDialog>
#include <QDebug>
#include <Objects/User/objects_app.h>
#include "models/model_visiting_control_all.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QMenu>

namespace Ui {
class Dialog_visiting_control;
}

class Dialog_visiting_control : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_visiting_control(QWidget *parent = 0);
    ~Dialog_visiting_control();
    model_visiting_control_all *model;

private:
    Ui::Dialog_visiting_control *ui;

signals:
    void get_medcard_id(QString, QString, int);

public slots:
    void load_model_sql();
    void view_green();
    void view_gray();
    void view_red();
    void view_all();
    void context_menu_table(QPoint pos);
    void add_cause();
    void selected_medcard_id();
};

#endif // DIALOG_VISITING_CONTROL_H
