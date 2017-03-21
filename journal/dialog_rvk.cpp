#include "dialog_rvk.h"
#include "ui_dialog_rvk.h"

Dialog_RVK::Dialog_RVK(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_RVK)
{
    ui->setupUi(this);
    connect(ui->pushButton_add,SIGNAL(clicked(bool)),SLOT(add_push_button()));
    connect(ui->pushButton_find,SIGNAL(clicked(bool)),SLOT(push_button_ok()));
    connect(ui->pushButton_delete,SIGNAL(clicked(bool)),SLOT(del_push_button()));
}

Dialog_RVK::~Dialog_RVK()
{
    delete ui;
}
void Dialog_RVK::add_push_button()
{
    Dialog_add_and_edit_journal dialog;
    if(dialog.exec())
    {
        //update table
    }
    else
    {
        //no update table
    }
}
void Dialog_RVK::edit_push_button()
{
//    int selected_tables = ui->tableView->currentIndex().row();
//    if (selected_tables >= 0)
//    {
//        int row = ui->tableView->currentIndex().row();
//        id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();

//    }

}
void Dialog_RVK::del_push_button()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление Пациента"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {
            query.exec("UPDATE test.journal_rvk SET delete_row='true' WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            push_button_ok();
        }

    }
    else
    {
        QMessageBox::warning(this,"Ошибка", "Выберете одного пациента");

    }
}
void Dialog_RVK::push_button_ok()
{
    find_journal(0);
}

void Dialog_RVK::find_journal(int key)
{
    delete model;
    model = nullptr;
    model = new QSqlTableModel();
    model->setTable("test.journal_rvk");
    switch (key) {
    case 0:
        model->setFilter("delete_row = 'false'");
        break;
    case 1:
        model->setFilter("delete_row = 'false' AND medcard_id = "+id_patient);
        break;
    }
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("№"));
    model->setHeaderData(1,Qt::Horizontal,tr("Дата создания записи"));
    model->setHeaderData(2,Qt::Horizontal,tr("Врач"));
    model->setHeaderData(4,Qt::Horizontal,tr("Улица"));
    model->setHeaderData(5,Qt::Horizontal,tr("Дом"));
    model->setHeaderData(6,Qt::Horizontal,tr("Корпус"));
    model->setHeaderData(7,Qt::Horizontal,tr("Квартира"));
    model->setHeaderData(8,Qt::Horizontal,tr("Решение"));
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(4);


}

void Dialog_RVK::input_signal(QString medcard_id)
{
  id_patient = medcard_id;
  find_journal(1);
}
