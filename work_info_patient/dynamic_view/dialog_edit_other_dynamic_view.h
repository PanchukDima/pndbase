#ifndef DIALOG_EDIT_OTHER_DYNAMIC_VIEW_H
#define DIALOG_EDIT_OTHER_DYNAMIC_VIEW_H

#include <QDialog>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "Objects/User/objects_app.h"

namespace Ui {
class Dialog_edit_other_dynamic_view;
}

class Dialog_edit_other_dynamic_view : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_edit_other_dynamic_view(QWidget *parent = 0);
    ~Dialog_edit_other_dynamic_view();
    QString global_id;
    int global_param;

private:
    Ui::Dialog_edit_other_dynamic_view *ui;
public slots:
    void setParam(int, QString);
    void load_data_sql();
    void save_data_sql();
    void hide_show_close_date();
};

#endif // DIALOG_EDIT_OTHER_DYNAMIC_VIEW_H
