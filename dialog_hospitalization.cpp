#include "dialog_hospitalization.h"
#include "ui_dialog_hospitalization.h"

Dialog_hospitalization::Dialog_hospitalization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_hospitalization)
{
    ui->setupUi(this);
    config_ui();
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
    connect(ui->checkBox,SIGNAL(stateChanged(int)),SLOT(check_change_check(int)));
}

Dialog_hospitalization::~Dialog_hospitalization()
{
    delete ui;
}
void Dialog_hospitalization::config_ui()
{

    ui->comboBox_who->addItem("СП","0");
    ui->comboBox_who->addItem("ПНД","1");
    ui->comboBox_who->addItem("Другое","2");
    ui->dateEdit_date_up->setDate(QDate::currentDate());
    ui->dateEdit_date_down->setDate(QDate::currentDate());
    if(ui->checkBox->isChecked())
    {
        ui->dateEdit_date_down->setEnabled(true);
    }
    else
    {
        ui->dateEdit_date_down->setEnabled(false);
    }

}
void Dialog_hospitalization::load_diagnos_patient()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString diagnos_code;
    if(db.open())
    {
        query.exec("SELECT diagnos_patient.id, diagnos.code  FROM test.diagnos ,test.diagnos_patient WHERE diagnos.id = diagnos_patient.diagnos_id AND  diagnos_patient.id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+global_id+" AND delete_row = 'false' AND id_parent is NULL)");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            dianose_id_patient = query.value(0).toString();
            diagnos_code = query.value(1).toString();
        }
        ui->label_4->setText("Диагноз № "+dianose_id_patient+" \nКод:"+diagnos_code);
    }
}

void Dialog_hospitalization::setParam(int param, QString id, QString staff)
{
    global_param = param;
    global_id = id;
    global_staff=staff;
    load_diagnos_patient();
    switch (param) {
    case 0:
        find_diagnos();
        break;
    case 1:
        ui->label_4->hide();
        get_data();
        break;
    }
}
void Dialog_hospitalization::find_diagnos()
{
}

void Dialog_hospitalization::check_change_check(int state)
{
    switch (state) {
    case 0:
        ui->dateEdit_date_down->setEnabled(false);
        break;
    case 2:
        ui->dateEdit_date_down->setEnabled(true);
        break;
    }
}

void Dialog_hospitalization::get_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT who_wrote, date_up, date_down FROM test.hospitalization WHERE id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            QString who = query.value(0).toString();
            QDate date_up = query.value(1).toDate();
            QDate date_down = query.value(2).toDate();
            if(query.value(2).toString()!="")
            {
                ui->checkBox->setChecked(true);
                ui->dateEdit_date_down->setEnabled(true);
            }
            else
            {
                ui->checkBox->setChecked(false);
                ui->dateEdit_date_down->setEnabled(false);
            }
            ui->comboBox_who->setCurrentIndex(ui->comboBox_who->findData(who));
            ui->dateEdit_date_up->setDate(date_up);
            ui->dateEdit_date_down->setDate(date_down);
        }
    }
}
void Dialog_hospitalization::send_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString who = ui->comboBox_who->currentData().toString();
    QString date_up = ui->dateEdit_date_up->date().toString("dd.MM.yyyy");
    QString date_down = ui->dateEdit_date_down->date().toString("dd.MM.yyyy");
    if(db.open())
    {
    switch (global_param) {
    case 0:
        if(ui->checkBox->checkState()==Qt::Checked)
        {
            query.exec("INSERT INTO test.hospitalization( medcard_id, staff_add_id, who_wrote, date_up, date_down, delete_row, diagnose_id) VALUES ( '"+global_id+"', '"+global_staff+"', '"+who+"', '"+date_up+"', '"+date_down+"', '0', "+dianose_id_patient+");");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена госпитализация в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        else
        {
            query.exec("INSERT INTO test.hospitalization( medcard_id, staff_add_id, who_wrote, date_up, delete_row, diagnose_id) VALUES ( '"+global_id+"', '"+global_staff+"', '"+who+"', '"+date_up+"', '0', "+dianose_id_patient+");");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена госпитализация в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }

        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена госпитализация в медкарту № "+global_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        Dialog_hospitalization::accept();
        break;
    case 1:
        if(ui->checkBox->checkState()==Qt::Checked)
        {
            query.exec("UPDATE test.hospitalization SET staff_add_id='"+global_staff+"', who_wrote='"+who+"', date_up='"+date_up+"', date_down='"+date_down+"' WHERE id="+global_id+";");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        else
        {
            query.exec("UPDATE test.hospitalization SET staff_add_id='"+global_staff+"', who_wrote='"+who+"', date_up='"+date_up+"', date_down=NULL WHERE id="+global_id+";");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена госпитализация № "+global_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        Dialog_hospitalization::accept();
        break;
    }

    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), "Потеряно соединение с сервером повторите попытку позднее", QMessageBox::Ok);
        Dialog_hospitalization::close();
    }
}

