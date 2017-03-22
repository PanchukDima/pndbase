#include "dialog_add_dynamic_view.h"
#include "ui_dialog_add_dynamic_view.h"
#include <QDebug>

Dialog_add_dynamic_view::Dialog_add_dynamic_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_dynamic_view)
{
    ui->setupUi(this);

    ui->radioButton_d->setChecked(true);
    ui->dateEdit_adn->setEnabled(false);
    ui->dateEdit_apl->setEnabled(false);
    ui->dateEdit_off_date->hide();
    ui->label_off_date->hide();
    block_state = false;

    connect(ui->radioButton_d,SIGNAL(toggled(bool)),SLOT(active_radio_button(bool)));

    connect(ui->radioButton_lp,SIGNAL(toggled(bool)),SLOT(active_radio_button_lp(bool)));
    connect(ui->checkBox_adn,SIGNAL(toggled(bool)),SLOT(active_adn_check(bool)));
    connect(ui->checkBox_apl,SIGNAL(toggled(bool)),SLOT(active_apl_check(bool)));
    connect(ui->pushButton_OK,SIGNAL(clicked()),SLOT(send_data()));
    connect(ui->pushButton_Cancel,SIGNAL(clicked()),SLOT(close()));
    connect(ui->checkBox_state,SIGNAL(stateChanged(int)),SLOT(show_hide_close_date()));

}

Dialog_add_dynamic_view::~Dialog_add_dynamic_view()
{
    delete ui;
}
void Dialog_add_dynamic_view::load_info_dynamic_view()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT on_date, group_disp_view, stop_date, status FROM test.dynamic_view WHERE id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        while(query.next())
        {
           ui->dateEdit->setDate(query.value(0).toDate());
           ui->radioButton_d->isChecked();

           switch (query.value(1).toInt()) {
           case 1:
               ui->radioButton_1->setChecked(true);
               break;
           case 2:
               ui->radioButton_2->setChecked(true);
               break;
           case 3:
               ui->radioButton_3->setChecked(true);
               break;
           case 4:
               ui->radioButton_4->setChecked(true);
               break;
           case 5:
               ui->radioButton_5->setChecked(true);
               break;
           case 6:
               ui->radioButton_6->setChecked(true);
               break;
           case 7:
               ui->radioButton_7->setChecked(true);
               break;
           }
           qDebug()<<query.value(1).toInt();
           ui->dateEdit_off_date->setDate(query.value(2).toDate());
           ui->checkBox_state->setChecked(query.value(3).toBool());

        }
        show_hide_close_date();
    }

}

void Dialog_add_dynamic_view::load_diagnos_patient()
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
        else
        {
        while(query.next())
        {
            dianose_id_patient = query.value(0).toString();
            diagnos_code = query.value(1).toString();
        }

        ui->label->setText("Диагноз № "+dianose_id_patient+" \nКод:"+diagnos_code);
        }
        }
}
void Dialog_add_dynamic_view::setParam(int param, QString id,QString staff)
{
    Dialog_add_dynamic_view::setWindowTitle("Добавление динамики наблюдения");
    global_param = param;
    global_staff = staff;
    global_id = id;
    load_diagnos_patient();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_adn->setDate(QDate::currentDate());
    ui->dateEdit_apl->setDate(QDate::currentDate());
    switch (param) {
    case 1:
        load_info_dynamic_view();
        break;
    }
}

void Dialog_add_dynamic_view::active_adn_check(bool state)
{
    if(state)
    {
        ui->dateEdit_adn->setEnabled(true);
    }
    else
    {
        ui->dateEdit_adn->setEnabled(false);
    }
}
void Dialog_add_dynamic_view::active_apl_check(bool state)
{
    if(state)
    {
        ui->dateEdit_apl->setEnabled(true);
    }
    else
    {
        ui->dateEdit_apl->setEnabled(false);
    }
}
void Dialog_add_dynamic_view::active_radio_button_k(bool q)
{
    if(q)
    {
        ui->radioButton_d->setChecked(false);
        ui->radioButton_lp->setChecked(false);
        ui->checkBox_adn->setCheckState(Qt::Unchecked);
        ui->checkBox_apl->setCheckState(Qt::Unchecked);
    }

}
void Dialog_add_dynamic_view::active_radio_button(bool q)
{
    if(q)
    {
        ui->groupBox_ambulance->setEnabled(true);

        ui->radioButton_lp->setChecked(false);
        ui->frame->setEnabled(true);
    }
    else
    {
        ui->groupBox_ambulance->setEnabled(false);
        ui->frame->setEnabled(false);
    }
}
void Dialog_add_dynamic_view::active_radio_button_lp(bool q)
{
    if(q)
    {
        ui->radioButton_d->setChecked(false);

        ui->checkBox_adn->setCheckState(Qt::Unchecked);
        ui->checkBox_apl->setCheckState(Qt::Unchecked);
    }

}
void Dialog_add_dynamic_view::getData()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT on_date, group_disp_view FROM test.dynamic_view WHERE status = 'false' AND delete_row='false' AND medcard_id = "+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {
        while(query.next())
        {
            QDate date_dinamic = query.value(0).toDate();
            int type_disp = query.value(1).toInt();
        }
        }
    }
}

void Dialog_add_dynamic_view::send_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString date_d = ui->dateEdit->date().toString("dd.MM.yyyy");
    QString date_apl = ui->dateEdit_apl->date().toString("dd.MM.yyyy");
    QString date_adn = ui->dateEdit_adn->date().toString("dd.MM.yyyy");
    QString date_visits;
    QString state;
    if(ui->checkBox_state->isChecked())
    {
        state = "true";
    }
    else
    {
        state = "false";
    }
    switch (global_param) {
    case 0:
    if(ui->radioButton_d->isChecked())
    {
        if(check_data(0))
        {
            QMessageBox::warning(this, tr("Ошибка"), "Уже открыта динамика наблюдения", QMessageBox::Ok);
            Dialog_add_dynamic_view::close();
        }
        else
        {

            if(ui->radioButton_1->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {

                 query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '1', 'false',"+dianose_id_patient+");");
                 if(query.lastError().isValid())
                 {
                     qDebug()<<query.lastError();
                     QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                 }
                 date_visits = ui->dateEdit->date().addDays(7).toString("dd.MM.yyyy");
                 query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                 if(query.lastError().isValid())
                 {
                     qDebug()<<query.lastError();
                     QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                 }
                 query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                 if(query.lastError().isValid())
                 {
                     qDebug()<<query.lastError();
                     QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                 }
                 //}
            }
            if(ui->radioButton_2->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '2', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                date_visits = ui->dateEdit->date().addDays(14).toString("dd.MM.yyyy");
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }

                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                // }
            }
            if(ui->radioButton_3->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '3', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                date_visits = ui->dateEdit->date().addMonths(1).toString("dd.MM.yyyy");
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                // }
            }
            if(ui->radioButton_4->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '4', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                date_visits = ui->dateEdit->date().addMonths(3).toString("dd.MM.yyyy");
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                // }
            }
            if(ui->radioButton_5->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '5', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                date_visits = ui->dateEdit->date().addMonths(6).toString("dd.MM.yyyy");
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                //}
            }
            if(ui->radioButton_6->isChecked())
            {
//                if(check_data(4))
//                {
//                    QMessageBox::warning(this, tr("Ошибка"), "У пациента открыта АДН или АПЛ", QMessageBox::Ok);
//                    Dialog_add_dynamic_view::close();
//                }
//                else
//                {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '6', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                date_visits = ui->dateEdit->date().addYears(1).toString("dd.MM.yyyy");
                query.exec("INSERT INTO test.visits_control(date_assigned, medcard_id, staff_add_id, delete_row) VALUES ( '"+date_visits+"', '"+global_id+"', '"+global_staff+"', 'false');");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                //}
            }

            if(ui->radioButton_7->isChecked())
            {
                 query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '7', 'false',"+dianose_id_patient+");");
                 if(query.lastError().isValid())
                 {
                     qDebug()<<query.lastError();
                     QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                 }
                 query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                 if(query.lastError().isValid())
                 {
                     qDebug()<<query.lastError();
                     QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                 }
            }
        }

    }
    if(ui->checkBox_adn->checkState()==Qt::Checked)
    {
        if(check_data(0))
        {

            if(check_data(1))
            {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_adn+"', 'false', '"+global_staff+"', '8', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), "Уже открыто АДН", QMessageBox::Ok);
                Dialog_add_dynamic_view::close();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Ошибка"), "Чтобы добавить АДН должно быть открыто диспансерное наблюдение", QMessageBox::Ok);
            Dialog_add_dynamic_view::close();

        }
    }
    if(ui->checkBox_apl->checkState()==Qt::Checked)
    {
        if(check_data(0))
        {

            if(check_data(2))
            {
                query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_apl+"', 'false', '"+global_staff+"', '9', 'false',"+dianose_id_patient+");");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), "Уже открыто АПЛ", QMessageBox::Ok);
                Dialog_add_dynamic_view::close();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Ошибка"), "Чтобы добавить АПЛ должно быть открыто диспансерное наблюдение", QMessageBox::Ok);
            Dialog_add_dynamic_view::close();
        }
    }
    if(ui->radioButton_lp->isChecked())
    {
        if(check_data(3))
        {
            QMessageBox::warning(this, tr("Ошибка"), "Уже открыта ЛП", QMessageBox::Ok);
            Dialog_add_dynamic_view::close();
        }
        else
        {
            qDebug()<<"INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '10', 'false',"+dianose_id_patient+");";
            query.exec("INSERT INTO test.dynamic_view( medcard_id, on_date, delete_row, staff_add_id, group_disp_view, status, diagnos_id) VALUES ( '"+global_id+"', '"+date_d+"', 'false', '"+global_staff+"', '10', 'false',"+dianose_id_patient+");");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена динамика наблюдения в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
    }

Dialog_add_dynamic_view::accept();
        break;
    case 1:
        if(db.open())
        {
            query.exec("UPDATE test.dynamic_view SET on_date='"+ui->dateEdit->date().toString("dd.MM.yyyy")+"',  group_disp_view='"+some_d()+"',stop_date='"+ui->dateEdit_off_date->date().toString("dd.MM.yyyy")+"', status='"+state+"' WHERE id = "+global_id);
            Dialog_add_dynamic_view::accept();
        }
        break;
    }
}
void Dialog_add_dynamic_view::global_block()
{
    QMessageBox::warning(this, tr("Ошибка"), block_message, QMessageBox::Ok);
    Dialog_add_dynamic_view::close();
}

bool Dialog_add_dynamic_view::check_data(int param)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    switch (param) {
    case 0:
        query.exec("SELECT count(*) FROM test.dynamic_view WHERE status='false' AND delete_row = 'false' AND group_disp_view IN('0','1','2','3','4','5','6','7') AND medcard_id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.next();
        if(query.value(0).toInt()>0)
        {
            return true;
        }
        else if(query.value(0).toInt()==0)
        {
            return false;
        }
        break;
    case 1:
        query.exec("SELECT count(*) FROM test.dynamic_view WHERE status='false' AND group_disp_view IN('8') AND medcard_id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.next();
        if(query.value(0).toInt()>0)
        {
            return false;
        }
        else if(query.value(0).toInt()==0)
        {
            return true;
        }
        break;
    case 2:
        query.exec("SELECT count(*) FROM test.dynamic_view WHERE status='false' AND group_disp_view IN('9') AND medcard_id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.next();
        if(query.value(0).toInt()>0)
        {
            return false;
        }
        else if(query.value(0).toInt()==0)
        {
            return true;
        }
        break;
    case 3:
        query.exec("SELECT count(*) FROM test.dynamic_view WHERE status='false' AND group_disp_view IN('10') AND medcard_id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.next();
        if(query.value(0).toInt()>0)
        {
            return true;
        }
        else if(query.value(0).toInt()==0)
        {
            return false;
        }
        break;
    case 4:
        query.exec("SELECT count(*) FROM test.dynamic_view WHERE status='false' AND group_disp_view IN('8','9') AND medcard_id="+global_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.next();
        if(query.value(0).toInt()>0)
        {
            return true;
        }
        else if(query.value(0).toInt()==0)
        {
            return false;
        }
        break;
    }
}

QString Dialog_add_dynamic_view::some_d()
{
    if(ui->radioButton_1->isChecked())
    {
        return "1";
    }
    if(ui->radioButton_2->isChecked())
    {
        return "2";
    }
    if(ui->radioButton_3->isChecked())
    {
        return "3";
    }
    if(ui->radioButton_4->isChecked())
    {
        return "4";
    }
    if(ui->radioButton_5->isChecked())
    {
        return "5";
    }
    if(ui->radioButton_6->isChecked())
    {
        return "6";
    }
    if(ui->radioButton_7->isChecked())
    {
        return "7";
    }

}
void Dialog_add_dynamic_view::show_hide_close_date()
{
    if(ui->checkBox_state->isChecked())
    {
        qDebug()<<"true show close_date";
        ui->dateEdit_off_date->show();
        ui->label_off_date->show();
    }
    else
    {
        qDebug()<<"false hide close_date";
        ui->dateEdit_off_date->hide();
        ui->label_off_date->hide();
    }
}
