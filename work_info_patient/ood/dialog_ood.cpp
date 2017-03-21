#include "dialog_ood.h"
#include "ui_dialog_ood.h"

Dialog_ood::Dialog_ood(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ood)
{
    ui->setupUi(this);
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
    ui->dateEdit_date->setDate(QDate::currentDate());
}

Dialog_ood::~Dialog_ood()
{
    delete ui;
}
void Dialog_ood::setParam(int param , QString id , QString staff_id)
{
    global_id = id;
    global_param = param;
    global_staff_id = staff_id;
    switch (param) {
    case 0:
        //add

        break;
    case 1:
        //edit
        load_data();
        break;
    }
}
void Dialog_ood::load_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT date_ood, criminal_article FROM test.ood WHERE id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            ui->dateEdit_date->setDate(query.value(0).toDate());
            ui->lineEdit_article->setText(query.value(1).toString());
        }
    }
}
void Dialog_ood::send_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date_value = ui->dateEdit_date->date().toString("dd.MM.yyyy");
    QString criminal_article = ui->lineEdit_article->text();

    if(db.open())
    {
        switch (global_param) {
        case 0:
            //insert

            query.exec("INSERT INTO test.ood(medcard_id, date_ood, criminal_article, staff_add_id, staff_edit_id, delete_row) VALUES ( "+global_id+", '"+date_value+"', '"+criminal_article+"', "+global_staff_id+", '0',  'false');");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен ООД в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            }
            Dialog_ood::accept();
            break;
        case 1:
            //update
            query.exec("UPDATE test.ood SET date_ood='"+date_value+"', criminal_article='"+criminal_article+"', staff_edit_id="+global_staff_id+" WHERE id = "+global_id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменен ООД № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            }
            Dialog_ood::accept();
            break;
        }
    }
}
