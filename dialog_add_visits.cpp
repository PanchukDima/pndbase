#include "dialog_add_visits.h"
#include "ui_dialog_add_visits.h"


Dialog_add_visits::Dialog_add_visits(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_visits)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(getData()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_add_visits::~Dialog_add_visits()
{
    delete ui;
}
void Dialog_add_visits::setParam(int param, QString id,QString staff)
{
    global_id=id;
    global_param=param;
    global_staff=staff;
    switch (param) {
    case 0:
        Dialog_add_visits::setWindowTitle("Добавление посещения");
        break;
    case 1:
        Dialog_add_visits::setWindowTitle("Изменение посещения");
        loadData();
        break;
    }

}
void Dialog_add_visits::loadData()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT date_assigned FROM test.visits_control WHERE id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit->setDate(query.value(0).toDate());
            }
        }
    }
}
void Dialog_add_visits::getData()
{
    QSqlDatabase db=QSqlDatabase::database();
    QSqlQuery query;
    QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
    if(ui->dateEdit->date()<QDate::currentDate())
    {
        QMessageBox::warning(this,"Запись в прошлое!","Нельзя записать человека задним числом");
        ui->dateEdit->setDate(QDate::currentDate());
    }
    else
    {
        if(db.open())
        {
            switch (global_param) {
            case 0:
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ('"+date+"', '"+global_id+"', '"+global_staff+"', '0')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлено посещение медкарта № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                Dialog_add_visits::accept();
                break;
            case 1:
                query.exec("UPDATE test.visits_control  SET date_assigned='"+date+"'  WHERE id= "+global_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменено посещение медкарта № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                Dialog_add_visits::accept();
                break;
            }

        }
    }
}
