#include "dialog_fast_find_patient_id.h"
#include "ui_dialog_fast_find_patient_id.h"

Dialog_fast_find_patient_id::Dialog_fast_find_patient_id(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_fast_find_patient_id)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),SLOT(find_patient()));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),SLOT(double_click_patient()));
}

Dialog_fast_find_patient_id::~Dialog_fast_find_patient_id()
{
    delete ui;
}
void Dialog_fast_find_patient_id::find_patient()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    if(db.open())
    {

        delete model;
        model = nullptr;
        model = new model_main_table();
        QString string_find = ui->lineEdit->text();
        QStringList list_string_find = string_find.split(" ");
        if(string_find != "")
        {
            for(int i = 0;list_string_find.count()>i;i++)
            {
                list_string_find[i].replace(" ","");
                QString string = list_string_find[i];
                string[0] = string[0].toUpper();
                list_string_find[i] = string;
            }
        }
        string_find = list_string_find.join(" ");
        model->setTable("patients_find");
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        if(string_find.toInt()==0)
        {
            //if(ui->checkBox_see_arhiv->isChecked())
            //{
                model->setFilter("concat LIKE '" + string_find + "%' AND area_id in("+obj.str_area_list+")");
            //}
           // else
           // {
            //    model->setFilter("concat LIKE '" + string_find + "%' AND area_id in("+area_append_user+") AND status in(100,101)");
            //}
        }
        else
        {
            //if(ui->checkBox_see_arhiv->isChecked())
            //{
                model->setFilter("patients_find.id = " + string_find + " AND area_id in("+obj.str_area_list+")");
            //}
//            else
//            {
//               model->setFilter("patients_find.id = " + string_find + " AND area_id in("+area_append_user+") AND status in(100,101)");
//            }
        }

        model->setHeaderData(2,Qt::Horizontal,tr("ФИО"));
        model->setHeaderData(3,Qt::Horizontal,tr("Серия\nПаспорта"));
        model->setHeaderData(4,Qt::Horizontal, tr("Номер\nПаспорта"));
        model->setHeaderData(5,Qt::Horizontal, tr("Дата\nРождения"));
        model->setHeaderData(7,Qt::Horizontal,tr("Участок"));
        model->setHeaderData(8,Qt::Horizontal,tr("Улица"));
        model->setHeaderData(9,Qt::Horizontal,tr("Дом"));
        model->setHeaderData(10,Qt::Horizontal,tr("Корпус"));
        model->setHeaderData(11,Qt::Horizontal,tr("Квартира"));
        model->setHeaderData(12,Qt::Horizontal,tr("Телефон"));
        model->select();

        ui->tableView->setModel(model);
        ui->tableView->hideColumn(6);
        ui->tableView->hideColumn(0);
        ui->tableView->hideColumn(1);
        ui->tableView->setColumnWidth(2,120);

        ui->tableView->resizeColumnsToContents();
    }
}

void Dialog_fast_find_patient_id::double_click_patient()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
    }
    Dialog_fast_find_patient_id::accept();
}
