#include "dialog_invalids_patient.h"
#include "ui_dialog_invalids_patient.h"

Dialog_invalids_patient::Dialog_invalids_patient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_invalids_patient)
{
    ui->setupUi(this);
    setting();
    connect(ui->checkBox_alltime,SIGNAL(stateChanged(int)),SLOT(check_state_changes(int)));
    connect(ui->lineEdit_date_view,SIGNAL(textChanged(QString)),SLOT(point_add_date(QString)));
    connect(ui->lineEdit_pereosved,SIGNAL(textChanged(QString)),SLOT(point_add_date_preosved(QString)));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_invalids_patient::~Dialog_invalids_patient()
{
    delete ui;
}
void Dialog_invalids_patient::setting()
{
    ui->comboBox_group_inv->addItem("", "-1");
    ui->comboBox_group_inv->addItem("I","0");
    ui->comboBox_group_inv->addItem("II","1");
    ui->comboBox_group_inv->addItem("III","2");
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, description  FROM test.type_work_place");
        while(query.next())
        {
            ui->comboBox_place_work->addItem(query.value(1).toString(),query.value(0).toString());
        }
        ui->comboBox_place_work->setCurrentIndex(ui->comboBox_place_work->findData("0"));
    }
}
void Dialog_invalids_patient::setParam(int param, QString id, QString staff)
{
    global_id=id;
    global_staff_id=staff;
    global_param = param;
    switch (param) {
    case 0:
        get_data();
        break;
    case 1:


        break;
    }
}
void Dialog_invalids_patient::check_state_changes(int state)
{
    if(state)
    {
        ui->label_4->hide();
        ui->lineEdit_pereosved->hide();
    }
    else
    {
        ui->label_4->show();
        ui->lineEdit_pereosved->show();
    }
}

void Dialog_invalids_patient::get_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs, inv_child FROM test.invalid_patient WHERE id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
        ui->lineEdit_date_view->setText(query.value(3).toDate().toString("dd.MM.yyyy"));
        ui->lineEdit_pereosved->setText(query.value(4).toDate().toString("dd.MM.yyyy"));
        ui->comboBox_group_inv->setCurrentIndex(ui->comboBox_group_inv->findData(query.value(0).toString()));
        ui->checkBox_alltime->setChecked(query.value(5).toBool());
        ui->checkBox_from_other->setChecked(query.value(2).toBool());
        ui->checkBox_invalid_children->setChecked(query.value(6).toBool());
        ui->checkBox_work_yes->setChecked(query.value(1).toBool());
        }
    }
}
void Dialog_invalids_patient::send_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QString group_invalid_value = ui->comboBox_group_inv->currentData().toString();
    QString from_other_value;
    QString work_yes_value;
    QString date_review_value;
    QString date_review_value_test = validate_date(ui->lineEdit_date_view->text());
    QString date_pereosved_value;
    QString date_pereosved_value_test = validate_date(ui->lineEdit_pereosved->text());
    QString inv_child_value;
    QString place_work = ui->comboBox_place_work->currentData().toString();
    QString dianose_id_patient;
    //получаем информацию о последнем диагнозе
    if(db.open())
    {
    query.exec("SELECT diagnos_patient.id, diagnos.code  FROM test.diagnos ,test.diagnos_patient WHERE diagnos.id = diagnos_patient.diagnos_id AND  diagnos_patient.id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+global_id+" AND delete_row = 'false' AND id_parent is NULL)");
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    else
    {
    while(query.next())
    {
        dianose_id_patient = query.value(0).toString();
    }
    }
    }


    if(ui->checkBox_invalid_children->isChecked())
    {
        inv_child_value = "true";
    }
    else
    {
        inv_child_value = "false";
    }

    if(date_review_value_test=="exit")
    {
        return;
    }
    else
    {
        date_review_value=date_review_value_test;
    }



    if(ui->checkBox_work_yes->checkState()==Qt::Checked)
    {
        work_yes_value="true";
    }
    else if (ui->checkBox_work_yes->checkState()==Qt::Unchecked)
    {
        work_yes_value="false";
    }

    if(ui->checkBox_from_other->checkState()==Qt::Checked)
    {
        from_other_value="true";
    }
    else if (ui->checkBox_from_other->checkState()==Qt::Unchecked)
    {
        from_other_value="false";
    }

    switch (global_param) {
    case 0:
        if(db.open())
        {
            if(ui->checkBox_alltime->checkState()==Qt::Checked)
            {

                query.exec("UPDATE test.invalid_patient SET staff_edit_id="+obj.staff_id+", group_inv="+group_invalid_value+", priznan_trudosp='"+work_yes_value+"', from_other='"+from_other_value+"', date_peresmotra='"+date_review_value+"', srok_ocherednogo_pereosvidet=NULL, bs='true', delete_row='false', inv_child='"+inv_child_value+"', place_work_id = "+place_work+" WHERE id ="+ global_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена инвалидность № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            else if (ui->checkBox_alltime->checkState()==Qt::Unchecked)
            {
                if(date_pereosved_value_test=="exit")
                {
                    return;
                }
                else
                {
                    date_pereosved_value=date_pereosved_value_test;
                }

                query.exec("UPDATE test.invalid_patient SET staff_edit_id="+obj.staff_id+", group_inv="+group_invalid_value+", priznan_trudosp='"+work_yes_value+"', from_other='"+from_other_value+"', date_peresmotra='"+date_review_value+"', srok_ocherednogo_pereosvidet='"+date_pereosved_value+"', bs='false', delete_row='false', inv_child='"+inv_child_value+"', place_work_id = "+place_work+"  WHERE id ="+ global_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена инвалидность № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            Dialog_invalids_patient::accept();
        }
        break;
    case 1:
        if(db.open())
        {
            if(ui->checkBox_alltime->checkState()==Qt::Checked)
            {

                //query.exec("UPDATE test.invalid_patient SET staff_edit_id="+obj.staff_id+", group_inv="+group_invalid_value+", priznan_trudosp='"+work_yes_value+"', from_other='"+from_other_value+"', date_peresmotra='"+date_review_value+"', srok_ocherednogo_pereosvidet=NULL, bs='true', delete_row='false', inv_child='"+inv_child_value+"', place_work_id = "+place_work+" WHERE id ="+ global_id);
                query.exec("INSERT INTO test.invalid_patient(\
                            medcard_id, \
                            staff_add_id,\
                            group_inv,\
                            priznan_trudosp, \
                            from_other,\
                            date_peresmotra,\
                            srok_ocherednogo_pereosvidet,\
                            bs, \
                            delete_row,\
                            inv_child,\
                            diagnos_id,\
                            place_work_id)\
                   VALUES\
                            (\
                            "+global_id+",\
                            "+obj.staff_id+",\
                            "+group_invalid_value+",\
                            '"+work_yes_value+"',\
                            '"+from_other_value+"',\
                            '"+date_review_value+"', \
                            NULL,\
                            'true',\
                            'false',\
                            '"+inv_child_value+"', \
                            "+dianose_id_patient+",\
                            "+place_work+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена инвалидность № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                Dialog_invalids_patient::accept();
            }
            else if (ui->checkBox_alltime->checkState()==Qt::Unchecked)
            {

                if(date_pereosved_value_test=="exit")
                {
                    return;
                }
                else
                {
                    date_pereosved_value=date_pereosved_value_test;
                }
                //query.exec("UPDATE test.invalid_patient SET staff_edit_id="+obj.staff_id+", group_inv="+group_invalid_value+", priznan_trudosp='"+work_yes_value+"', from_other='"+from_other_value+"', date_peresmotra='"+date_review_value+"', srok_ocherednogo_pereosvidet='"+date_pereosved_value+"', bs='false', delete_row='false', inv_child='"+inv_child_value+"', place_work_id = "+place_work+"  WHERE id ="+ global_id);
                query.exec("INSERT INTO test.invalid_patient(\
                            medcard_id, \
                            staff_add_id,\
                            group_inv,\
                            priznan_trudosp, \
                            from_other,\
                            date_peresmotra,\
                            srok_ocherednogo_pereosvidet,\
                            bs, \
                            delete_row,\
                            inv_child,\
                            diagnos_id,\
                            place_work_id)\
                   VALUES\
                            (\
                            "+global_id+",\
                            "+obj.staff_id+",\
                            "+group_invalid_value+",\
                            '"+work_yes_value+"',\
                            '"+from_other_value+"',\
                            '"+date_review_value+"', \
                            '"+date_pereosved_value+"',\
                            'false',\
                            'false',\
                            '"+inv_child_value+"', \
                            "+dianose_id_patient+",\
                            "+place_work+");");

                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменена инвалидность № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                Dialog_invalids_patient::accept();
            }
        }
        break;
    default:
        Dialog_invalids_patient::close();
    }


}
QString const Dialog_invalids_patient::validate_date(QString date)
{

    QStringList day_list = date.split(".");
    QString day;
    QString month;
    QString year;
    QDate date_valid;
    QString date_valid_string;
    if(day_list.count()==3)
    {
        day = day_list[0];
        month = day_list[1];
        year = day_list[2];
        if(date_valid.setDate(year.toInt(),month.toInt(),day.toInt()))
        {
            date_valid_string = date_valid.toString("dd.MM.yyyy");
            return date_valid_string;
        }
        else
        {
            QMessageBox::information(this,"Не правильная дата","Не правильная дата");
            return "exit";
        }
    }
    else
    {
        QMessageBox::information(this,"Не правильный формат даты","Не правильный формат даты");
        return "exit";

    }
}
void Dialog_invalids_patient::point_add_date(QString text)
{
    switch (text.count()) {
    case 2:
        if(text.lastIndexOf(".")!=1)
        {
        ui->lineEdit_date_view->setText(text+".");
        }
        break;
    case 5:

        if(text.lastIndexOf(".")!=4)
        {
            ui->lineEdit_date_view->setText(text+".");
        }
            break;
    case 11:
        text.replace(10,1,"");
        ui->lineEdit_date_view->setText(text);
        break;

    }
}
void Dialog_invalids_patient::point_add_date_preosved(QString text)
{
    switch (text.count()) {
    case 2:
        if(text.lastIndexOf(".")!=1)
        {
        ui->lineEdit_pereosved->setText(text+".");
        }
        break;
    case 5:

        if(text.lastIndexOf(".")!=4)
        {
            ui->lineEdit_pereosved->setText(text+".");
        }
            break;
    case 11:
        text.replace(10,1,"");
        ui->lineEdit_pereosved->setText(text);
        break;


    }
}
