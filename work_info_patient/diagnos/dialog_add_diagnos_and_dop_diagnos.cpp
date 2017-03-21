#include "dialog_add_diagnos_and_dop_diagnos.h"
#include "ui_dialog_add_diagnos_and_dop_diagnos.h"

Dialog_add_diagnos_and_dop_diagnos::Dialog_add_diagnos_and_dop_diagnos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_diagnos_and_dop_diagnos)
{
    ui->setupUi(this);
    load_allow_data();
    ui->label_date_dop->hide();
    ui->dateEdit_date_dop_diagnos->hide();
    connect(ui->pushButton_create,SIGNAL(clicked(bool)),SLOT(add_main_diagnos()));
    connect(ui->pushButton_add_dop_diagnos,SIGNAL(clicked(bool)),SLOT(add_dop_diagnos()));
    connect(ui->comboBox_code_dop_diagnos,SIGNAL(activated(int)),SLOT(changes_diagnos_code_dop(int)));
    connect(ui->comboBox_code_main_diagnos,SIGNAL(activated(int)),SLOT(changes_diagnos_code_main(int)));
    connect(ui->comboBox_name_dop_diagnos,SIGNAL(activated(int)),SLOT(changes_diagnos_name_dop(int)));
    connect(ui->comboBox_name_main_diagnos,SIGNAL(activated(int)),SLOT(changes_diagnos_name_main(int)));
    connect(ui->pushButton_close,SIGNAL(clicked(bool)),SLOT(accept()));
}

Dialog_add_diagnos_and_dop_diagnos::~Dialog_add_diagnos_and_dop_diagnos()
{
    delete ui;
}

void Dialog_add_diagnos_and_dop_diagnos::load_allow_data()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
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

            ui->comboBox_code_main_diagnos->addItem(code_diagnos, id_diagnos);
            ui->comboBox_code_dop_diagnos->addItem(code_diagnos, id_diagnos);
            ui->comboBox_name_main_diagnos->addItem(diagnos, id_diagnos);
            ui->comboBox_name_dop_diagnos->addItem(diagnos, id_diagnos);

        }
        ui->dateEdit_main->setDate(QDate::currentDate());
    }
}
void Dialog_add_diagnos_and_dop_diagnos::setParam(QString id, QString staff_id)
{
    Dialog_add_diagnos_and_dop_diagnos::setWindowTitle("Добавление диагноза в медкарту №"+id);
    global_id = id;
    staff_add_id = staff_id;
}
void Dialog_add_diagnos_and_dop_diagnos::add_main_diagnos()
{
    Objects_app obj;
    QString date_fixing_diagnos = ui->dateEdit_main->date().toString("dd.MM.yyyy");
    QString diagnos_id = ui->comboBox_name_main_diagnos->currentData().toString();
    QString first_value;
    if(ui->checkBox_first->isChecked())
    {
        first_value = "true";
    }
    else
    {
        first_value = "false";
    }
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
        if(db.open())
        {
            qDebug()<<"INSERT INTO test.diagnos_patient(medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date, delete_row,first) VALUES ('"+global_id+"', '"+diagnos_id+"', '"+obj.staff_id+"', '"+date_fixing_diagnos+"', 'false', '"+first_value+"') RETURNING id";
            query.exec("INSERT INTO test.diagnos_patient(medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date, delete_row,first) VALUES ('"+global_id+"', '"+diagnos_id+"', '"+obj.staff_id+"', '"+date_fixing_diagnos+"', 'false', '"+first_value+"') RETURNING id");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while(query.next())
            {
                diagnos_id_add = query.value(0).toString();
            }
         }
        if(db.open())
        {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен диагноз в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        if(diagnos_id_add.toInt()>0)
        {
            ui->groupBox_2->setEnabled(true);
            ui->pushButton_create->setEnabled(false);
            ui->label_status->setText("Статус: Диагноз добавлен под номером "+diagnos_id_add);
            ui->dateEdit_date_dop_diagnos->setDate(ui->dateEdit_main->date());
        }
}
void Dialog_add_diagnos_and_dop_diagnos::add_dop_diagnos()
{
    Objects_app obj;
    QString date_fixing_diagnos = ui->dateEdit_main->date().toString("dd.MM.yyyy");
    QString diagnos_id = ui->comboBox_name_dop_diagnos->currentData().toString();
    QString id_add;
        QSqlDatabase db = QSqlDatabase::database();
            QSqlQuery query;
            if(db.open())
            {
                query.exec("INSERT INTO test.diagnos_patient(medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date, id_parent, delete_row) VALUES ('"+global_id+"', '"+diagnos_id+"', '"+obj.staff_id+"', '"+date_fixing_diagnos+"', '"+diagnos_id_add+"', 'false') RETURNING id");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                while(query.next())
                {
                   id_add = query.value(0).toString();
                }
            }
            if(db.open())
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлен сопутствующий диагноз в медкарту № "+global_id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            if(id_add.toInt()>0)
            {
                ui->label_status_dop->setText("Статус: Диагноз добавлен под номером "+ id_add);
                ui->pushButton_add_dop_diagnos->setEnabled(false);
            }

}
void Dialog_add_diagnos_and_dop_diagnos::changes_diagnos_code_dop(int index_code)
{
    QString id_diagnos = ui->comboBox_code_dop_diagnos->itemData(index_code).toString();
    ui->comboBox_name_dop_diagnos->setCurrentIndex(ui->comboBox_name_dop_diagnos->findData(id_diagnos));
}
void Dialog_add_diagnos_and_dop_diagnos::changes_diagnos_code_main(int index_code)
{
    QString id_diagnos = ui->comboBox_code_main_diagnos->itemData(index_code).toString();
    ui->comboBox_name_main_diagnos->setCurrentIndex(ui->comboBox_name_main_diagnos->findData(id_diagnos));
}
void Dialog_add_diagnos_and_dop_diagnos::changes_diagnos_name_dop(int index_code)
{
    QString id_diagnos = ui->comboBox_name_dop_diagnos->itemData(index_code).toString();
    ui->comboBox_code_dop_diagnos->setCurrentIndex(ui->comboBox_code_dop_diagnos->findData(id_diagnos));
}
void Dialog_add_diagnos_and_dop_diagnos::changes_diagnos_name_main(int index_code)
{
    QString id_diagnos = ui->comboBox_name_main_diagnos->itemData(index_code).toString();
    ui->comboBox_code_main_diagnos->setCurrentIndex(ui->comboBox_code_main_diagnos->findData(id_diagnos));
}
