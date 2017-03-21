#include "dialog_diagnos_selected_patient.h"
#include "ui_dialog_diagnos_selected_patient.h"



Dialog_diagnos_selected_patient::Dialog_diagnos_selected_patient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_diagnos_selected_patient)
{
    ui->setupUi(this);

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),SLOT(double_click_table()));

}

Dialog_diagnos_selected_patient::~Dialog_diagnos_selected_patient()
{
    delete ui;
}
void Dialog_diagnos_selected_patient::select_diagnos_patient()
{
    QSqlTableModel *model = new QSqlTableModel();
    qDebug()<<medcard_id;
    model->setTable("test.diagnos_patient_info");
    model->setFilter("delete_row = 'false' AND id_parent is null AND medcard_id = "+medcard_id);
    model->select();
    ui->tableView->setModel(model);

}
void Dialog_diagnos_selected_patient::double_click_table()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        diagnos_id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        diagnos_name = ui->tableView->model()->index(row,2).data(Qt::DisplayRole).toString();
        diagnos_code = ui->tableView->model()->index(row, 6).data(Qt::DisplayRole).toString();
    }
    Dialog_diagnos_selected_patient::accept();
}
void Dialog_diagnos_selected_patient::setParam(QString id)
{
    medcard_id = id;
    select_diagnos_patient();
}
