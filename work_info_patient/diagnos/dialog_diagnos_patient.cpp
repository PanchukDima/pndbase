#include "dialog_diagnos_patient.h"
#include "ui_dialog_diagnos_patient.h"

Dialog_diagnos_patient::Dialog_diagnos_patient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_diagnos_patient)
{
    ui->setupUi(this);
    load_allow_date();
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->comboBox_code_diagnos,SIGNAL(activated(int)),SLOT(change_code_diagnos(int)));
    connect(ui->comboBox_diagnos,SIGNAL(activated(int)),SLOT(change_description_diagnos(int)));
}

Dialog_diagnos_patient::~Dialog_diagnos_patient()
{
    delete ui;
}

void Dialog_diagnos_patient::load_allow_date()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
            query.exec("SELECT diagnos.id, diagnos.name, diagnos.code FROM test.diagnos ORDER BY diagnos.code ASC;");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while (query.next())
    {
        QString id_diagnos = query.value(0).toString();
        QString diagnos = query.value(1).toString();
        QString code_diagnos = query.value(2).toString();
        ui->comboBox_code_diagnos->addItem(code_diagnos, id_diagnos);
        ui->comboBox_diagnos->addItem(diagnos, id_diagnos);

    }
            ui->dateEdit_date_fixing_diagnos->setDate(QDate::currentDate());
    }
}

void Dialog_diagnos_patient::setParam(int param, QString id, QString staff_id)
{
    global_param = param;
    global_id = id;
    global_staff_id = staff_id;
    switch (param) {
    case 0:
        //add diagnos
        Dialog_diagnos_patient::setWindowTitle("Добавление диагноза");
        break;
    case 1:
        //edit diagnos
        get_data();
        Dialog_diagnos_patient::setWindowTitle("Изменение диагноза");
        break;
    case 2:
        //add children diagnos
        Dialog_diagnos_patient::setWindowTitle("Добавление сопутствующего диагноза");
        break;
    }
}
void Dialog_diagnos_patient::get_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    if(db.open())
    {
        query.exec("SELECT \
                   diagnos_patient.fixing_diagnos_date,\
                   diagnos_patient.diagnos_id , \
                   diagnos_patient.first, \
                   diagnos_patient.id_parent\
                   FROM \
                   test.diagnos_patient\
                   WHERE \
                   diagnos_patient.id = "+global_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                while(query.next())
        {
                QDate fixing_diagnos_date_value = query.value(0).toDate();
                QString diagnos_id = query.value(1).toString();
                ui->checkBox_first->setChecked(query.value(2).toBool());
                QString id_parent = query.value(3).toString();
                if(id_parent=="")
        {
                ui->checkBox_first->setEnabled(false);
    }
                ui->dateEdit_date_fixing_diagnos->setDate(fixing_diagnos_date_value);
                ui->comboBox_diagnos->setCurrentIndex(ui->comboBox_diagnos->findData(diagnos_id));

    }
    }

}
void Dialog_diagnos_patient::send_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString first_value;
    Objects_app obj;
    if(ui->checkBox_first->isChecked())
    {
        first_value = "true";
    }
    else
    {
        first_value = "false";
    }
    qDebug()<<first_value;
    QString date_fixing_diagnos = ui->dateEdit_date_fixing_diagnos->date().toString("dd.MM.yyyy");
    QString diagnos_id = ui->comboBox_diagnos->currentData().toString();
    switch (global_param) {
    case 0:
        //insert
        query.exec("INSERT INTO test.diagnos_patient(medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date, delete_row, first) VALUES ('"+global_id+"', '"+diagnos_id+"', '"+obj.staff_id+"', '"+date_fixing_diagnos+"', 'false', first='"+first_value+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен диагноз в медкарту № "+global_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        }
        Dialog_diagnos_patient::accept();
        break;
    case 1:
        //update

        query.exec("UPDATE test.diagnos_patient SET  diagnos_id='"+diagnos_id+"', staff_add_id='"+obj.staff_id+"', fixing_diagnos_date='"+date_fixing_diagnos+"', first='"+first_value+"' WHERE id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменен диагноз № "+global_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        }
        Dialog_diagnos_patient::accept();
        break;
    case 2:
        QString medcard_id;
        query.exec("SELECT diagnos_patient.medcard_id FROM test.diagnos_patient WHERE diagnos_patient.id ="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            medcard_id = query.value(0).toString();
        }

        query.exec("INSERT INTO test.diagnos_patient(medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date, id_parent,delete_row ,first) VALUES ('"+medcard_id+"', '"+diagnos_id+"', '"+obj.staff_id+"', '"+date_fixing_diagnos+"', '"+global_id+"', 'false', '"+first_value+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {


        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен сопутствующий диагноз в медкарту № "+medcard_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        }
        Dialog_diagnos_patient::accept();
        break;
    }
}
void Dialog_diagnos_patient::change_code_diagnos(int index_code)
{
    QString id_diagnos = ui->comboBox_code_diagnos->itemData(index_code).toString();
    ui->comboBox_diagnos->setCurrentIndex(ui->comboBox_diagnos->findData(id_diagnos));
}
void Dialog_diagnos_patient::change_description_diagnos(int index_code)
{
    QString id_diagnos = ui->comboBox_diagnos->itemData(index_code).toString();
    ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(id_diagnos));
}
