#include "dialog_list_not_work.h"
#include "ui_dialog_list_not_work.h"

Dialog_list_not_work::Dialog_list_not_work(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_list_not_work)
{
    ui->setupUi(this);
    load_lists();
    show_or_hide_close(true);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(send_data()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->dateEdit_close,SIGNAL(dateChanged(QDate)),SLOT(date_close_edit(QDate)));
    connect(ui->checkBox_close,SIGNAL(toggled(bool)),SLOT(show_or_hide_close(bool)));
    connect(ui->comboBox_code_diagnos,SIGNAL(activated(int)),SLOT(changes_code_diagnos(int)));
    connect(ui->comboBox_name_diagnos,SIGNAL(activated(int)),SLOT(changes_name_diagnos(int)));

}

Dialog_list_not_work::~Dialog_list_not_work()
{
    delete ui;
}
void Dialog_list_not_work::load_lists()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT id, who FROM test.who_closed;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
            QString id = query.value(0).toString();
            QString description = query.value(1).toString();
            ui->comboBox_who_open->addItem(description,id);
            ui->comboBox_who_close->addItem(description,id);
        }
    }
    if(db.open())
    {
            query.exec("SELECT diagnos.id, diagnos.name, diagnos.code, diagnos.staff_add_id FROM test.diagnos ORDER BY diagnos.code ASC;");
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
        ui->comboBox_name_diagnos->addItem(diagnos, id_diagnos);
    }
    }
    if(db.open())
    {
        ui->comboBox_doctor_open->addItem("Отсутствует", "0");
        ui->comboBox_doctor_close->addItem("Отсутствует", "0");
        query.exec("SELECT id, concat(fname, ' ',name, ' ' ,mname) FROM test.staff WHERE staff.position = '1'");
        while(query.next())
        {
            ui->comboBox_doctor_open->addItem(query.value(1).toString(), query.value(0).toString());
            ui->comboBox_doctor_close->addItem(query.value(1).toString(), query.value(0).toString());
        }

    }

}
void Dialog_list_not_work::load_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT date_start, date_stop, diagnos_id, who_open, who_close, state, doctor_open, doctor_close FROM test.notwork_list WHERE id =" +global_id);
        while(query.next())
        {
            ui->dateEdit_open->setDate(query.value(0).toDate());
            ui->checkBox_close->setChecked(query.value(5).toBool());
            if(query.value(5).toBool())
            {
                ui->dateEdit_close->setDate(query.value(1).toDate());
                ui->comboBox_who_close->setCurrentIndex(ui->comboBox_who_close->findData(query.value(4).toString()));
                show_or_hide_close(true);
            }
            else
            {
                show_or_hide_close(false);
                date_count_restart(0);
            }
            ui->comboBox_who_open->setCurrentIndex(ui->comboBox_who_open->findData(query.value(3).toString()));
            ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(query.value(2).toString()));
            ui->comboBox_doctor_open->setCurrentIndex(ui->comboBox_doctor_open->findData(query.value(6).toString()));
            ui->comboBox_doctor_close->setCurrentIndex(ui->comboBox_doctor_close->findData(query.value(7).toString()));
        }
    }
}
void Dialog_list_not_work::load_current_diagnos()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString diagnos_id;
    if(db.open())
    {
        query.exec("SELECT diagnos_patient.diagnos_id FROM test.diagnos_patient WHERE id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+global_id+" AND delete_row = 'false');");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            diagnos_id = query.value(0).toString();
        }
        ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(diagnos_id));
        ui->comboBox_name_diagnos->setCurrentIndex(ui->comboBox_name_diagnos->findData(diagnos_id));
    }

}

void Dialog_list_not_work::setParam(int param, QString id)
{
    global_id = id;
    global_param = param;

    switch (param) {
    case 0:
        //add
        ui->comboBox_who_close->hide();
        ui->label_close->hide();
        ui->dateEdit_close->hide();
        ui->label_who_close->hide();
        ui->dateEdit_open->setDate(QDate::currentDate());
        ui->dateEdit_close->setDate(QDate::currentDate());
        this->setWindowTitle("Добавление листа нетрудоспособности");
        break;
    case 1:
        //edit
        load_data();
        ui->dateEdit_close->setDate(QDate::currentDate());
        this->setWindowTitle("Изменение листа нетрудоспособности");
        break;
    }
}
void Dialog_list_not_work::send_data()
{
    Objects_app obj;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString day_count = ui->label_day_count->text();
    QString date_open_value = ui->dateEdit_open->date().toString("dd.MM.yyyy");
    QString date_close_value = ui->dateEdit_close->date().toString("dd.MM.yyyy");
    QString doctor_open = ui->comboBox_doctor_open->currentData().toString();
    QString doctor_close = ui->comboBox_doctor_close->currentData().toString();
    bool state = ui->checkBox_close->isChecked();
    QString state_str;
    QString diagnos_id = ui->comboBox_code_diagnos->currentData().toString();
    QString who_open = ui->comboBox_who_open->currentData().toString();
    QString who_close = ui->comboBox_who_close->currentData().toString();
    if(state)
    {
        state_str = "true";
    }
    else
    {
        state_str = "false";
    }
    if(db.open())
    {
        switch (global_param) {
        case 0:
                if(ui->checkBox_close->isChecked())
                {
                    query.exec("INSERT INTO test.notwork_list(date_start, date_stop, medcard_id, count_day, staff_add_id, staff_edit_id, delete_row, diagnos_id, who_open, who_close, state, doctor_open, doctor_close) VALUES ('"+date_open_value+"', '"+date_close_value+"', "+global_id+", "+day_count+", "+obj.staff_id+", 0, 'false', "+diagnos_id+", "+who_open+", "+who_close+", '"+state_str+"', "+doctor_open+", "+doctor_close+");");
                }
                else
                {
                    query.exec("INSERT INTO test.notwork_list(date_start, date_stop, medcard_id, count_day, staff_add_id, staff_edit_id, delete_row, diagnos_id, who_open, who_close, state, doctor_open, doctor_close) VALUES ('"+date_open_value+"', NULL, "+global_id+", NULL, "+obj.staff_id+", 0, 'false', "+diagnos_id+", "+who_open+", NULL, '"+state_str+"', "+doctor_open+", 0);");
                }

              if(query.lastError().isValid())
              {
                  qDebug()<<query.lastError();
                  QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
              }
              else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ("+obj.staff_id+", '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен больничный лист в медкарту "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                else
                {
                    Dialog_list_not_work::accept();
                }
            }


            break;
        case 1:
            if(ui->checkBox_close->isChecked())
            {
                query.exec("UPDATE test.notwork_list SET date_start='"+date_open_value+"', date_stop='"+date_close_value+"', count_day="+day_count+", staff_edit_id="+obj.staff_id+", diagnos_id="+diagnos_id+", who_open="+who_open+", who_close="+who_close+", state='"+state_str+"', doctor_open="+doctor_open+", doctor_close="+doctor_close+" WHERE id = "+global_id+";");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            else
            {
                query.exec("UPDATE test.notwork_list SET date_start='"+date_open_value+"', date_stop=NULL, count_day=NULL, staff_edit_id="+obj.staff_id+", diagnos_id="+diagnos_id+", who_open="+who_open+", who_close="+who_close+", state='"+state_str+"', doctor_open="+doctor_open+" WHERE id = "+global_id+";");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }

            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ("+obj.staff_id+", '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Изменен больничный лист "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            Dialog_list_not_work::accept();
            break;
        }
    }

}
void Dialog_list_not_work::date_close_edit(QDate date)
{
    QDate date_open = ui->dateEdit_open->date();
    int day = date_open.daysTo(date);
    day = day + 1;
    QString day_count;
    if(ui->checkBox_close->isChecked())
    {
        ui->label_day_count->setText(day_count.setNum(day));
    }
    else
    {
        ui->label_day_count->setText("");
    }
}
void Dialog_list_not_work::show_or_hide_close(bool state)
{
    if(state)
    {
        ui->comboBox_who_close->show();
        ui->label_close->show();
        ui->dateEdit_close->show();
        ui->label_who_close->show();
        date_count_restart(1);
        ui->label_doctor_close->show();
        ui->comboBox_doctor_close->show();
    }
    else
    {
        ui->comboBox_who_close->hide();
        ui->label_close->hide();
        ui->dateEdit_close->hide();
        ui->label_who_close->hide();
        date_count_restart(0);
        ui->label_doctor_close->hide();
        ui->comboBox_doctor_close->hide();
    }
}
void Dialog_list_not_work::changes_code_diagnos(int index_code)
{
    QString id_diagnos = ui->comboBox_code_diagnos->itemData(index_code).toString();
    ui->comboBox_name_diagnos->setCurrentIndex(ui->comboBox_name_diagnos->findData(id_diagnos));
}
void Dialog_list_not_work::changes_name_diagnos(int index_code)
{
    QString id_diagnos = ui->comboBox_name_diagnos->itemData(index_code).toString();
    ui->comboBox_code_diagnos->setCurrentIndex(ui->comboBox_code_diagnos->findData(id_diagnos));
}
void Dialog_list_not_work::date_count_restart(int type)
{
    QDate date_open = ui->dateEdit_open->date();
    QDate date_close = ui->dateEdit_close->date();
    int day = date_open.daysTo(date_close);
    day = day + 1;
    QString day_count;
    if(type==0)
    {
        ui->label_day_count->setText("");
    }
    else
    {
        ui->label_day_count->setText(day_count.setNum(day));
    }
}
