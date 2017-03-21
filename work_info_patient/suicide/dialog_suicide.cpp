#include "dialog_suicide.h"
#include "ui_dialog_suicide.h"

Dialog_suicide::Dialog_suicide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_suicide)
{
    ui->setupUi(this);
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
}

Dialog_suicide::~Dialog_suicide()
{
    delete ui;
}
void Dialog_suicide::load_lists()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT tsuicide.id, tsuicide.description FROM test.tsuicide;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {
            QString id_suicide = query.value(0).toString();
            QString suicide = query.value(1).toString();
            ui->comboBox_type_suicide->addItem(suicide, id_suicide);
        }
        ui->dateEdit_suicide->setDate(QDate::currentDate());
    }
}

void Dialog_suicide::setParam(int param ,QString id, QString staff_id)
{
    global_id = id;
    global_staff_id = staff_id;
    global_param = param;
    switch (param) {
    case 0:
        //Добавить
        Dialog_suicide::setWindowTitle("Добавление информации о суциде");
        load_lists();
        break;
    case 1:
        //Изменить
        Dialog_suicide::setWindowTitle("Изменение информации о суциде");
        load_lists();
        load_data();
        break;
    }

}
void Dialog_suicide::load_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT suicide.datestart, suicide.tsuicide_id FROM test.suicide WHERE suicide.id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            ui->dateEdit_suicide->setDate(query.value(0).toDate());
            ui->comboBox_type_suicide->setCurrentIndex(ui->comboBox_type_suicide->findData(query.value(1).toString()));
        }
    }
}
void Dialog_suicide::send_data()
{
    QString date_value = ui->dateEdit_suicide->date().toString("dd.MM.yyyy");
    QString type_value = ui->comboBox_type_suicide->currentData().toString();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    switch (global_param) {
    case 0:
        if(db.open())
        {

            query.exec("INSERT INTO test.suicide( datestart, tsuicide_id, staff_add_id, staff_edit_id, medcard_id, delete_row) VALUES ( '"+date_value+"', "+type_value+", "+global_staff_id+", 0, "+global_id+", 'false');");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен суицид в медкарту № "+global_id+"')");

            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            }
            }
        Dialog_suicide::accept();
        break;
    case 1:
        if(db.open())
        {
            query.exec("UPDATE test.suicide SET datestart='"+date_value+"', tsuicide_id="+type_value+", staff_edit_id='"+global_staff_id+"' WHERE id = "+global_id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменен суицид № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            }
            }
        Dialog_suicide::accept();
        break;
    }
}
