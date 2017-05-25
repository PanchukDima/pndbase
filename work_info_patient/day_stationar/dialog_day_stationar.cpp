#include "dialog_day_stationar.h"
#include "ui_dialog_day_stationar.h"

Dialog_day_stationar::Dialog_day_stationar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_day_stationar)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(get_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_day_stationar::~Dialog_day_stationar()
{
    delete ui;
}
void Dialog_day_stationar::setParam(int param, QString id)
{
    global_id = id;
    global_param = param;
    switch (param) {
    case 0:
        this->setWindowTitle("Постановка на диспансер");
        ui->dateEdit_day_in->setDate(QDate::currentDate());
        ui->dateEdit_day_out->setDate(QDate::currentDate());
        break;
    case 1:
        load_sql();
        this->setWindowTitle("Изменение записи на диспансер");
        ui->dateEdit_day_out->setDate(QDate::currentDate());
        break;
    }
}
void Dialog_day_stationar::get_sql()
{
    Objects_app obj;
    QString date_in_value = ui->dateEdit_day_in->date().toString("dd.MM.yyyy");
    QString date_out_value = ui->dateEdit_day_out->date().toString("dd.MM.yyyy");
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        switch (global_param) {
        case 0:
            if(ui->checkBox_close->isChecked())
            {
                query.exec("INSERT INTO test.day_stationar( day_in, day_out, staff_add_id,staff_edit_id, medcard_id, delete_row, this_close) VALUES ('"+date_in_value+"', '"+date_out_value+"',0, "+obj.staff_id+", "+global_id+", false, true);");
            }
            else
            {
                query.exec("INSERT INTO test.day_stationar( day_in, day_out, staff_add_id, staff_edit_id, medcard_id, delete_row, this_close) VALUES ('"+date_in_value+"', NULL, "+obj.staff_id+",0, "+global_id+", false, false);");
            }
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"SQL-ошибка","Произошла ошибка обращения к базе данных");
                Dialog_day_stationar::close();
            }
            else
            {
                Dialog_day_stationar::accept();
            }
            break;
        case 1:
            if(ui->checkBox_close->isChecked())
            {
                query.exec("UPDATE test.day_stationar SET day_in='"+date_in_value+"', day_out='"+date_out_value+"', staff_edit_id="+obj.staff_id+", this_close=true WHERE id="+global_id);
            }
            else
            {
                query.exec("UPDATE test.day_stationar SET day_in='"+date_in_value+"', day_out=NULL, staff_edit_id="+obj.staff_id+", this_close=false WHERE id="+global_id);
            }
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"SQL- ошибка","Произошла ошибка обращения к базе данных");
                Dialog_day_stationar::close();
            }
            else
            {
                Dialog_day_stationar::accept();
            }
            break;
        }
    }
}
void Dialog_day_stationar::load_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT day_in, day_out, this_close FROM test.day_stationar WHERE id ="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit_day_in->setDate(query.value(0).toDate());
                if(query.value(2).toBool())
                {
                    ui->dateEdit_day_out->setDate(query.value(1).toDate());
                }
                else
                {
                    ui->dateEdit_day_out->setDate(QDate::currentDate());
                }
            }
        }
    }
}
