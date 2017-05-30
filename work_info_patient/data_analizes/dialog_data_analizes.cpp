#include "dialog_data_analizes.h"
#include "ui_dialog_data_analizes.h"

Dialog_data_analizes::Dialog_data_analizes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_data_analizes)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(get_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_data_analizes::~Dialog_data_analizes()
{
    delete ui;
}
void Dialog_data_analizes::load_library()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, name  FROM library.analizes;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->comboBox_type->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }
}
void Dialog_data_analizes::load_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT date_appointment, type_analizes FROM test.data_analizes WHERE id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit_appoint->setDate(query.value(0).toDate());
                ui->comboBox_type->setCurrentIndex(ui->comboBox_type->findData(query.value(1).toString()));
            }
        }
    }
}
void Dialog_data_analizes::get_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QString date_appoint = ui->dateEdit_appoint->date().toString("dd.MM.yyyy");
    QString type = ui->comboBox_type->currentData().toString();
    if(db.open())
    {
        switch (global_param) {
        case 0:
            query.exec("INSERT INTO test.data_analizes( medcard_id, staff_add_id, staff_edit_id, staff_del_id, delete_row, date_appointment, type_analizes) VALUES ("+global_id+", "+obj.staff_id+", 0, 0, false,'"+date_appoint+"', "+type+");");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                Dialog_data_analizes::close();
            }
            else
            {
                Dialog_data_analizes::accept();
            }
            break;
        case 1:
            query.exec("UPDATE test.data_analizes SET  staff_edit_id="+obj.staff_id+", date_appointment='"+date_appoint+"', type_analizes="+type+" WHERE id ="+global_id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                Dialog_data_analizes::close();
            }
            else
            {
                Dialog_data_analizes::accept();
            }
            break;
        }
    }
}
void Dialog_data_analizes::setParam(int param, QString id)
{
    global_id = id;
    global_param = param;
    switch (param) {
    case 0:
        ui->dateEdit_appoint->setDate(QDate::currentDate());
        load_library();
        break;
    case 1:
        load_library();
        load_sql();
        break;
    }
}
