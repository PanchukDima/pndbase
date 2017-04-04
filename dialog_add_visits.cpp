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
    connect(ui->checkBox_status_visits,SIGNAL(stateChanged(int)),SLOT(show_hide_visits_close()));
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
        query.exec("SELECT date_assigned, date_presence FROM test.visits_control WHERE id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit->setDate(query.value(0).toDate());
                if(query.value(1).toDate().isNull())
                {
                    ui->checkBox_status_visits->setChecked(false);
                }
                else
                {
                    ui->checkBox_status_visits->setChecked(true);
                    ui->dateEdit_visits->setDate(query.value(1).toDate());
                }
                show_hide_visits_close();
            }
        }
    }
}
void Dialog_add_visits::getData()
{
    QSqlDatabase db=QSqlDatabase::database();
    QSqlQuery query;
    QString date = ui->dateEdit->date().toString("dd.MM.yyyy");
//    if(ui->dateEdit->date()<QDate::currentDate())
//    {
//        QMessageBox::warning(this,"Запись в прошлое!","Нельзя записать человека задним числом");
//        ui->dateEdit->setDate(QDate::currentDate());
//        return;
//    }

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

//                if(ui->dateEdit_visits->date()>QDate::currentDate())
//                {
//                    QMessageBox::warning(this,"Не правильная дата закрытия", "Этот день еще не наступил");
//                    return;
//                }
                if(ui->checkBox_status_visits->isChecked())
                {
                    query.exec("UPDATE test.visits_control  SET date_assigned='"+date+"', date_presence='"+ui->dateEdit_visits->date().toString("dd.MM.yyyy")+"'  WHERE id= "+global_id);
                }
                else
                {
                    query.exec("UPDATE test.visits_control  SET date_assigned='"+date+"', date_presence=NULL  WHERE id= "+global_id);
                }
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
void Dialog_add_visits::show_hide_visits_close()
{
    if(ui->checkBox_status_visits->isChecked())
    {
        ui->label_close_visits->show();
        ui->dateEdit_visits->show();
    }
    else
    {
        ui->label_close_visits->hide();
        ui->dateEdit_visits->hide();
    }
}
