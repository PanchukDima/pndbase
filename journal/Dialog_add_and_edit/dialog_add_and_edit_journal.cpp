#include "dialog_add_and_edit_journal.h"
#include "ui_dialog_add_and_edit_journal.h"

Dialog_add_and_edit_journal::Dialog_add_and_edit_journal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_and_edit_journal)
{
    ui->setupUi(this);
    init_other_data();
    connect(ui->pushButton_select_patient,SIGNAL(clicked(bool)),SLOT(open_dialog_select_patient()));
    connect(ui->pushButton_select_daignos,SIGNAL(clicked(bool)),SLOT(open_dialog_select_diagnos()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(get_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_add_and_edit_journal::~Dialog_add_and_edit_journal()
{
    delete ui;
}
void Dialog_add_and_edit_journal::open_dialog_select_patient()
{
    Dialog_fast_find_patient_id dialog;
    if(dialog.exec())
    {
        ui->lineEdit_number_medcard->setText(dialog.id);
        id_patient = dialog.id;
        dialog.deleteLater();
        init_find_patient();
    }
    else
    {
        qDebug()<<"Ошибка открытия";
    }
    //Далее sql запросы для формы
}
void Dialog_add_and_edit_journal::init_find_patient()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QStringList fio;
    if(db.open())
    {
        query.exec("SELECT \
                   patient.fname,\
                   patient.name, \
                   patient.mname, \
                   street.name, \
                   address_patient.building, \
                   address_patient.flat, \
                   address_patient.home,\
                   address_patient.id,  \
                   address_patient.street_id\
                   FROM \
                   test.medcard, \
                   test.patient, \
                   test.address_patient,\
                   test.street\
                   WHERE \
                   patient.id = medcard.patient_id AND\
                address_patient.medcard_id = medcard.id AND\
                street.id = address_patient.street_id AND\
                medcard.id = "+id_patient);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                while(query.next())
        {
                fio.append(query.value(0).toString());
                fio.append(query.value(1).toString());
                fio.append(query.value(2).toString());

                ui->lineEdit_street_name->setText(query.value(3).toString());
                ui->lineEdit_house->setText(query.value(4).toString());
                ui->lineEdit_building->setText(query.value(5).toString());
                ui->lineEdit_flat->setText(query.value(6).toString());

                street_id = query.value(8).toString();
                house = query.value(4).toString();
                building = query.value(5).toString();
                flat = query.value(6).toString();
    }
                ui->lineEdit_number_medcard->setText(id_patient);
                ui->lineEdit_FIO_patient->setText(fio.join(" "));
        if(ui->lineEdit_FIO_patient->text()=="")
        {
            ui->pushButton_select_daignos->setEnabled(true);
        }
    }
}


void Dialog_add_and_edit_journal::init_other_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, concat(fname, ' ',name, ' ' ,mname) FROM test.staff WHERE staff.position = '1';");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            ui->comboBox_doctor->addItem(query.value(1).toString(),query.value(0).toString());
        }

    }
    ui->comboBox_result->addItem("Д","0");
    ui->comboBox_result->addItem("ЛП","1");
    ui->comboBox_result->addItem("Архив","2");
    ui->comboBox_result->addItem("Другое","3");


}
void Dialog_add_and_edit_journal::open_dialog_select_diagnos()
{
    Dialog_diagnos_selected_patient dialog_diagnos;
    if(id_patient != ui->lineEdit_number_medcard->text())
    {
        return;
    }
    dialog_diagnos.setParam(ui->lineEdit_number_medcard->text());
    if(dialog_diagnos.exec())
    {
        id_diagnos = dialog_diagnos.diagnos_id;
        name_diagnos = dialog_diagnos.diagnos_name;
        code_diagnos = dialog_diagnos.diagnos_code;
    }
    ui->lineEdit_name_diagnos->setText(name_diagnos);
    ui->lineEdit_number_diagnos->setText(id_diagnos);

}
void Dialog_add_and_edit_journal::input_signal(QString medcard_id)
{
    id_patient = medcard_id;
    ui->pushButton_select_daignos->setEnabled(true);
    init_find_patient();
}
void Dialog_add_and_edit_journal::get_sql()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date = ui->dateEdit_date_input->date().toString("dd.MM.yyyy");
    QString doctor = ui->comboBox_doctor->currentData().toString();
    QString result = ui->comboBox_result->currentData().toString();
    QString staff = obj.staff_id;

    if(db.open())
    {
        query.exec("INSERT INTO test.journal_rvk(\
                   date_creat_record, doctor, medcard_id, street_id, house,\
                   building, flat, diagnos_id, result, staff_add_id)\
                VALUES ('"+date+"', "+doctor+", "+id_patient+", "+street_id+", "+house+",\
                        "+building+", "+flat+", "+code_diagnos+",'"+result+"', "+staff+");");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
    }

    Dialog_add_and_edit_journal::accept();
}
void Dialog_add_and_edit_journal::setParam(int param, QString id)
{
    edit_id = id;
    switch (param) {
        case 1:
        load_sql();
        break;
    case 2:
        load_in_journal();
        break;
    }
}
void Dialog_add_and_edit_journal::load_in_journal()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec();
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {

        }
    }
}

void Dialog_add_and_edit_journal::update_sql()
{
    //ui->pushButton_select_daignos->hide();
    //ui->pushButton_select_patient->hide();
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date = ui->dateEdit_date_input->date().toString("dd.MM.yyyy");
    QString doctor = ui->comboBox_doctor->currentData().toString();
    QString result = ui->comboBox_result->currentData().toString();
    QString staff = obj.staff_id;

    if(db.open())
    {
        query.exec("UPDATE test.journal_rvk\
           SET date_creat_record='"+date+"', doctor='"+doctor+"', medcard_id='"+id_patient+"', street_id="+street_id+",\
               house='"+house+"', building='"+building+"', flat='"+flat+"', diagnos_id="+code_diagnos+", result='"+result+"', staff_edit_id="+staff+" WHERE id = "+edit_id);


                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
    }
}
void Dialog_add_and_edit_journal::load_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, date_creat_record, doctor, medcard_id, street_id, house, \
                   building, flat, diagnos_id, result, who_signed_predsed, who_signed_zav_otdel,\
                   who_signed_doctor, when_signed, name_responsible, staff_add_id, \
                   staff_edit_id, delete_row, time_stamp\
                   FROM test.journal_rvk;");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                while(query.next())
        {
            ui->dateEdit_date_input->setDate(query.value(1).toDate());
            id_patient = query.value(3).toString();
            ui->comboBox_result->currentData(ui->comboBox_result->findData(query.value(9).toString()));
            init_find_patient();
        }


    }
}

