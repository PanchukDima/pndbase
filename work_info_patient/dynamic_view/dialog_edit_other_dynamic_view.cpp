#include "dialog_edit_other_dynamic_view.h"
#include "ui_dialog_edit_other_dynamic_view.h"

Dialog_edit_other_dynamic_view::Dialog_edit_other_dynamic_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_edit_other_dynamic_view)
{
    ui->setupUi(this);
    connect(ui->checkBox_close,SIGNAL(stateChanged(int)),SLOT(hide_show_close_date()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(save_data_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_edit_other_dynamic_view::~Dialog_edit_other_dynamic_view()
{
    delete ui;
}
void Dialog_edit_other_dynamic_view::setParam(int param, QString id)
{
    global_id = id;
    global_param = param;
    Dialog_edit_other_dynamic_view::setWindowTitle("Изменение динамики наблюдения");
    ui->dateEdit_open->setDate(QDate::currentDate());
    ui->dateEdit_close->setDate(QDate::currentDate());
    switch (param) {
    case 1:
        load_data_sql();
        break;
    }
}
void Dialog_edit_other_dynamic_view::load_data_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT on_date,status, stop_date FROM test.dynamic_view WHERE id ="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
            ui->dateEdit_open->setDate(query.value(0).toDate());
            ui->dateEdit_close->setDate(query.value(2).toDate());
            ui->checkBox_close->setChecked(query.value(1).toBool());
            }
        }
    }
    hide_show_close_date();

}
void Dialog_edit_other_dynamic_view::save_data_sql()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date_open = ui->dateEdit_open->date().toString("dd.MM.yyyy");
    QString date_close = ui->dateEdit_close->date().toString("dd.MM.yyyy");
    QString close_state;
    if(ui->checkBox_close->isChecked())
    {
        close_state = "true";
    }
    else
    {
        close_state = "false";
    }
    if(db.open())
    {
    switch (global_param) {
    case 1:
        if(ui->checkBox_close->isChecked())
        {
            query.exec("UPDATE test.dynamic_view SET on_date='"+date_open+"', status='true',stop_date='"+date_close+"' WHERE id ="+global_id);

        }
        else
        {
            query.exec("UPDATE test.dynamic_view SET on_date='"+date_open+"', status='false', stop_date = NULL WHERE id ="+global_id);

        }
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена динамика наблюдения № "+global_id+"')");
            Dialog_edit_other_dynamic_view::accept();
        }
        break;
    }
    }
    else
    {
        qDebug()<<"Server lost";
    }
}
void Dialog_edit_other_dynamic_view::hide_show_close_date()
{
    if(ui->checkBox_close->isChecked())
    {
        ui->label_calose_date->show();
        ui->dateEdit_close->show();
    }
    else
    {
        ui->label_calose_date->hide();
        ui->dateEdit_close->hide();
    }
}
