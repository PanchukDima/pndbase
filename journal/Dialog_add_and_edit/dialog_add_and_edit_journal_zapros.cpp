#include "dialog_add_and_edit_journal_zapros.h"
#include "ui_dialog_add_and_edit_journal_zapros.h"

Dialog_add_and_edit_journal_zapros::Dialog_add_and_edit_journal_zapros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_and_edit_journal_zapros)
{
    ui->setupUi(this);
    connect(ui->pushButton_add_contact,SIGNAL(clicked(bool)),SLOT(open_add_contact()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(get_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->comboBox_code_diagnos,SIGNAL(currentIndexChanged(int)),SLOT(code_diagnos_edit()));
    connect(ui->comboBox_name_diagnos,SIGNAL(currentIndexChanged(int)),SLOT(name_diagnos_edit()));
}

Dialog_add_and_edit_journal_zapros::~Dialog_add_and_edit_journal_zapros()
{
    delete ui;
}
void Dialog_add_and_edit_journal_zapros::load_library_contact()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    ui->comboBox_contact->clear();
    if(db.open())
    {
        query.exec("SELECT id, name FROM library.contact_list");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while (query.next())
            {
                ui->comboBox_contact->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }
}

void Dialog_add_and_edit_journal_zapros::load_library()
{
    load_library_contact();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    if(db.open())
    {
        query.exec("SELECT id, name FROM test.area;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while (query.next())
            {
                ui->comboBox_area->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }
    if(db.open())
    {
        query.exec("SELECT id, name, code FROM test.diagnos;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->comboBox_code_diagnos->addItem(query.value(2).toString(),query.value(0).toString());
                ui->comboBox_name_diagnos->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }

    }
    if(db.open())
    {
        query.exec("SELECT id, name FROM test.street;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->comboBox_street->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }

    if(db.open())
    {
        query.exec("SELECT id, concat(fname,' ', name,' ' ,mname)  FROM test.staff;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->comboBox_doctor_answer->addItem(query.value(1).toString(),query.value(0).toString());
            }
        }
    }
    ui->comboBox_who_sign->addItem("Караулов О.А.","1");
    ui->comboBox_who_sign->addItem("Саренко А.Н.","2");
    ui->comboBox_who_sign->addItem("Николаева Е.Н.","3");
    ui->comboBox_who_sign->addItem("Другой вариант","4");
}
void Dialog_add_and_edit_journal_zapros::get_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date_in_doc = ui->dateEdit_input_doc->date().toString("dd.MM.yyyy");
    QString number_in_doc = ui->lineEdit_input_number_doc->text();
    QString who_send = ui->comboBox_contact->currentData().toString();
    QString fname = ui->lineEdit_fname->text();
    QString name = ui->lineEdit_name->text();
    QString oname = ui->lineEdit_oname->text();
    QString birthday = ui->dateEdit_birthday->date().toString("dd.MM.yyyy");
    QString street = ui->comboBox_street->currentData().toString();
    QString building = ui->lineEdit_building->text();
    QString house = ui->lineEdit_house->text();
    QString flat = ui->lineEdit_flat->text();
    QString area_id = ui->comboBox_area->currentData().toString();
    QString diagnos_id = ui->comboBox_code_diagnos->currentData().toString();
    QString doctor = ui->comboBox_doctor_answer->currentData().toString();
    QString who_sign = ui->comboBox_who_sign->currentData().toString();
    QString date_sign = ui->dateEdit_sign->date().toString("dd.MM.yyyy");
    QString date_out_doc = ui->dateEdit_out_doc->date().toString("dd.MM.yyyy");
    QString number_out_doc = ui->lineEdit_out_number_doc->text();

    switch (global_param) {
    case 0:
        if(db.open())
        {
            query.exec("INSERT INTO test.journal_zapros( in_date_doc, in_number_doc, who_send, fname_p, name_p, oname_p,birthday_p, street_id, building_p, house_p, flat_p, area_id, diagnos_id, doctor_id, who_signed, date_signed, out_date_doc, out_number_doc, staff_id_respons)VALUES ( '"+date_in_doc+"', "+number_in_doc+", "+who_send+", '"+fname+"', '"+name+"', '"+oname+"', '"+birthday+"', "+street+", "+building+", "+house+", "+flat+", "+area_id+", "+diagnos_id+", "+doctor+", "+who_sign+", '"+date_sign+"', '"+date_out_doc+"', "+number_out_doc+",0);");
            Dialog_add_and_edit_journal_zapros::accept();
        }
        break;
    case 1:
        if(db.open())
        {
            query.exec("UPDATE test.journal_zapros SET in_date_doc='"+date_in_doc+"', in_number_doc="+number_in_doc+", who_send="+who_send+", fname_p='"+fname+"', name_p='"+name+"', oname_p='"+oname+"', birthday_p='"+birthday+"', street_id="+street+", building_p="+building+",house_p="+house+", flat_p="+flat+", area_id="+area_id+", diagnos_id="+diagnos_id+", doctor_id="+doctor+", who_signed="+who_sign+", date_signed='"+date_sign+"', out_date_doc='"+date_out_doc+"', out_number_doc="+number_out_doc+" WHERE id="+global_id);
            Dialog_add_and_edit_journal_zapros::accept();
        }
        break;
    }

}
void Dialog_add_and_edit_journal_zapros::load_sql()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT in_date_doc,\
                         in_number_doc,\
                              who_send,\
                               fname_p,\
                                name_p,\
                               oname_p,\
                            birthday_p,\
                             street_id,\
                            building_p,\
                               house_p,\
                                flat_p,\
                               area_id,\
                            diagnos_id,\
                             doctor_id,\
                            who_signed,\
                           date_signed,\
                          out_date_doc,\
                        out_number_doc,\
                      staff_id_respons \
                      FROM test.journal_zapros WHERE id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        else
        {
            while(query.next())
            {
                ui->dateEdit_input_doc->setDate(query.value(0).toDate());
                ui->lineEdit_input_number_doc->setText(query.value(1).toString());
                ui->comboBox_contact->setCurrentIndex(ui->comboBox_contact->findData(query.value(2).toString()));
                ui->lineEdit_fname->setText(query.value(3).toString());
                ui->lineEdit_name->setText(query.value(4).toString());
                ui->lineEdit_oname->setText(query.value(5).toString());
                ui->dateEdit_birthday->setDate(query.value(6).toDate());
                ui->comboBox_street->setCurrentIndex(ui->comboBox_street->findData(query.value(7).toString()));
                ui->lineEdit_building->setText(query.value(8).toString());
                ui->lineEdit_house->setText(query.value(9).toString());
                ui->lineEdit_flat->setText(query.value(10).toString());
                ui->comboBox_area->setCurrentIndex(ui->comboBox_area->findData(query.value(11).toString()));
                ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(query.value(12).toString()));
                ui->comboBox_name_diagnos->setCurrentIndex(ui->comboBox_name_diagnos->findData(query.value(12).toString()));
                ui->comboBox_doctor_answer->setCurrentIndex(ui->comboBox_doctor_answer->findData(query.value(13).toString()));
                ui->comboBox_who_sign->setCurrentIndex(ui->comboBox_who_sign->findData(query.value(14).toString()));
                ui->dateEdit_sign->setDate(query.value(15).toDate());
                ui->dateEdit_out_doc->setDate(query.value(16).toDate());
                ui->lineEdit_out_number_doc->setText(query.value(17).toString());

            }
        }
    }

}
void Dialog_add_and_edit_journal_zapros::setParam(int param)
{
    global_param = param;
    load_library();
    switch (param) {
    case 0:
        ui->dateEdit_sign->setDate(QDate::currentDate());
        ui->dateEdit_out_doc->setDate(QDate::currentDate());
        ui->dateEdit_input_doc->setDate(QDate::currentDate());
        ui->dateEdit_birthday->setDate(QDate::currentDate());
        break;
    case 1:

        break;
    }
}
void Dialog_add_and_edit_journal_zapros::open_add_contact()
{
    Dialog_add_contact dialog;
    if(dialog.exec())
    {
        load_library_contact();
    }
}
void Dialog_add_and_edit_journal_zapros::edit(QString id)
{
    global_param = 1;
    global_id = id;
    load_library();
    load_sql();
}
void Dialog_add_and_edit_journal_zapros::code_diagnos_edit()
{
    QString id = ui->comboBox_code_diagnos->currentData().toString();
    ui->comboBox_name_diagnos->setCurrentIndex(ui->comboBox_name_diagnos->findData(id));
}
void Dialog_add_and_edit_journal_zapros::name_diagnos_edit()
{
    QString id = ui->comboBox_name_diagnos->currentData().toString();
    ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(id));
}
