#include "dialog_zapros.h"
#include "ui_dialog_zapros.h"

Dialog_zapros::Dialog_zapros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_zapros)
{
    ui->setupUi(this);
    this->showMaximized();
    load_table_param();
    connect(ui->pushButton_add,SIGNAL(clicked(bool)),SLOT(add_zapros()));
    connect(ui->pushButton_edit,SIGNAL(clicked(bool)),SLOT(edit_zapros()));
}

Dialog_zapros::~Dialog_zapros()
{
    delete ui;
}
void Dialog_zapros::add_zapros()
{
    Dialog_add_and_edit_journal_zapros dialog;
    dialog.setParam(0);
    if(dialog.exec())
    {
        model->select();
    }
}
void Dialog_zapros::edit_zapros()
{
    Dialog_add_and_edit_journal_zapros dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.edit(id);
        if(dialog.exec())
        {
            model->select();
        }
    }
}
void Dialog_zapros::del_zapros()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        int selected_tables = ui->tableView->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView->currentIndex().row();
            QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
            query.exec("UPDATE test.journal_zapros SET delete_row = true WHERE id="+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }

        }
    }
}
void Dialog_zapros::load_table_param()
{
    model = new model_journal_zapros();
    model->setTable("test.journal_zapros_view");
    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Дата вх. док."));
    model->setHeaderData(2,Qt::Horizontal,tr("№ вх. док."));
    model->setHeaderData(3,Qt::Horizontal,tr("От кого"));
    model->setHeaderData(4,Qt::Horizontal,tr("Фамилия"));
    model->setHeaderData(5,Qt::Horizontal,tr("Имя"));
    model->setHeaderData(6,Qt::Horizontal,tr("Отчество"));
    model->setHeaderData(7,Qt::Horizontal,tr("Дата рождения"));
    model->setHeaderData(8,Qt::Horizontal,tr("улица"));
    model->setHeaderData(9,Qt::Horizontal,tr("Корпус"));
    model->setHeaderData(10,Qt::Horizontal,tr("Дом"));
    model->setHeaderData(11,Qt::Horizontal,tr("Квартира"));
    model->setHeaderData(12,Qt::Horizontal,tr("Участок"));
    model->setHeaderData(13,Qt::Horizontal,tr("Диагноз"));
    model->setHeaderData(14,Qt::Horizontal,tr("Доктор"));
    model->setHeaderData(15,Qt::Horizontal,tr("Кто\nподписал"));
    model->setHeaderData(16,Qt::Horizontal,tr("Дата подписи"));
    model->setHeaderData(17,Qt::Horizontal,tr("Дата исх. док."));
    model->setHeaderData(18,Qt::Horizontal,tr("№ исх. док."));
    model->setHeaderData(19,Qt::Horizontal,tr("Удалена"));
    model->setFilter("delete_row = false");
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(19);
}
