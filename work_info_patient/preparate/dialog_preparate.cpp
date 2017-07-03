#include "dialog_preparate.h"
#include "ui_dialog_preparate.h"

Dialog_Preparate::Dialog_Preparate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Preparate)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(get_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_Preparate::~Dialog_Preparate()
{
    delete ui;
}
void Dialog_Preparate::get_sql()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date_appointment = ui->dateEdit_appointment->date().toString("dd.MM.yyyy");
    QString date_vk = ui->dateEdit_vk->date().toString("dd.MM.yyyy");
    QString preparete_id = ui->comboBox_preparate_name->currentData().toString();
    switch (global_param) {
    case 0:
        qDebug()<<"INSERT INTO test.preparate( staff_add_id, deleted_row, staff_edit_id, medcard_id, date_first_appointment, date_vk, preparate_id) VALUES ("+obj.staff_id+", false, 0, "+global_id+", '"+date_appointment+"', '"+date_vk+"', "+preparete_id+");";
        query.exec("INSERT INTO test.preparate( staff_add_id, deleted_row, staff_edit_id, medcard_id, date_first_appointment, date_vk, preparate_id) VALUES ("+obj.staff_id+", false, 0, "+global_id+", '"+date_appointment+"', '"+date_vk+"', "+preparete_id+");");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            Dialog_Preparate::close();
        }
        else
        {
            Dialog_Preparate::accept();
        }
        break;
    case 1:
        query.exec("UPDATE test.preparate SET  staff_edit_id="+obj.staff_id+", date_first_appointment='"+date_appointment+"',date_vk='"+date_vk+"', preparate_id="+preparete_id+" WHERE id ="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            Dialog_Preparate::close();
        }
        else
        {
            Dialog_Preparate::accept();
        }
        break;
    }

}
void Dialog_Preparate::load_library()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, name FROM library.preparate_list order by name");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this, "SQL-error","Ошибка 300");
        }
        else
        {
            while(query.next())
            {
                ui->comboBox_preparate_name->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }
}
void Dialog_Preparate::load_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT date_first_appointment, date_vk, preparate_id FROM test.preparate WHERE medcard_id ="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            Dialog_Preparate::close();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit_appointment->setDate(query.value(0).toDate());
                ui->dateEdit_vk->setDate(query.value(1).toDate());
                ui->comboBox_preparate_name->setCurrentIndex(ui->comboBox_preparate_name->findData(query.value(2).toString()));
            }
        }

    }
}
void Dialog_Preparate::setParam(int param, QString id)
{
    global_param = param;
    global_id = id;
    switch (param) {
    case 0:
        load_library();
        ui->dateEdit_appointment->setDate(QDate::currentDate());
        ui->dateEdit_vk->setDate(QDate::currentDate());
        break;
    case 1:
        load_library();
        load_sql();
        break;
    }
}
