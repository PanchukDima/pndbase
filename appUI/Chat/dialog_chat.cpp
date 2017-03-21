#include "dialog_chat.h"
#include "ui_dialog_chat.h"

Dialog_chat::Dialog_chat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_chat)
{
    ui->setupUi(this);
    connect(ui->tableView_staff,SIGNAL(clicked(QModelIndex)),SLOT(loadMessages()));
    connect(ui->tableView_group,SIGNAL(clicked(QModelIndex)),SLOT(loadMessages()));
    connect(ui->pushButton_send,SIGNAL(clicked(bool)),SLOT(sendMessages()));
    init_start();

}

Dialog_chat::~Dialog_chat()
{
    delete ui;
}

void Dialog_chat::loadMessages()
{
    Objects_app obj;
    if(ui->tableView_staff->isVisible())
    {
        int selected_tables = ui->tableView_staff->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_staff->currentIndex().row();
            QString id = ui->tableView_staff->model()->index(row,0).data(Qt::DisplayRole).toString();
            model_chat->setTable("messager.messages_view");
            model_chat->setFilter("staff_to = "+ obj.staff_id+" AND staff_from = "+ id);
            model_chat->select();
            id_message = id;
            select_type_message = 0;
            if(model_chat->lastError().isValid())
            {
                qDebug()<<model_chat->lastError();
            }
        }
    }
    if(ui->tableView_group->isVisible())
    {
        int selected_tables = ui->tableView_group->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_group->currentIndex().row();
            QString id = ui->tableView_group->model()->index(row,0).data(Qt::DisplayRole).toString();
            model_chat->setTable("messager.messages_groups");
            model_chat->setFilter("group_id = "+id);
            model_chat->select();
            id_message = id;
            select_type_message = 1;
            if(model_chat->lastError().isValid())
            {
                qDebug()<<model_chat->lastError();
            }

        }
    }
    ui->tableView_chat->hideColumn(0);
    ui->tableView_chat->hideColumn(1);
    ui->tableView_chat->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableView_chat->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->tableView_chat->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);


}
void Dialog_chat::sendMessages()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        switch (select_type_message) {
        case 0:
            query.exec("INSERT INTO messager.messages(staff_from, staff_to, message) VALUES ("+id_message+", "+obj.staff_id+", '"+ui->lineEdit_message->text()+"');");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            ui->lineEdit_message->clear();
            model_chat->select();
            break;
        case 1:
            query.exec("INSERT INTO messager.group_messages(group_id, staff_add_id, message,) VALUES ("+id_message+", "+obj.staff_id+", '"+ui->lineEdit_message->text()+"');");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            ui->lineEdit_message->clear();
            model_chat->select();
            break;
        }
    }
}
void Dialog_chat::loadGroups()
{
    model_groups->select();
}
void Dialog_chat::loadStaff()
{
   model_staff->select();
}
void Dialog_chat::init_start()
{
    Objects_app obj;
    model_staff = new QSqlTableModel();
    model_groups = new QSqlTableModel();
    model_chat = new QSqlTableModel();

    model_staff->setTable("test.staff_view");
    model_groups->setTable("messager.group");


    model_groups->setFilter(obj.staff_id + "= ANY(staff_id)");
    model_staff ->setFilter("id>0");

    model_staff->select();
    model_groups->select();


    if(model_staff->lastError().isValid())
    {
        qDebug()<<model_staff->lastError();
    }
    if(model_groups->lastError().isValid())
    {
        qDebug()<<model_groups->lastError();
    }

    ui->tableView_staff->setModel(model_staff);
    ui->tableView_group->setModel(model_groups);
    ui->tableView_chat->setModel(model_chat);

    ui->tableView_staff->hideColumn(0);
    ui->tableView_staff->hideColumn(2);
    ui->tableView_staff->hideColumn(3);
    ui->tableView_staff->hideColumn(4);
    ui->tableView_group->hideColumn(0);
    ui->tableView_group->hideColumn(2);
    ui->tableView_chat->hideColumn(0);
    ui->tableView_chat->hideColumn(1);

    ui->tableView_staff->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView_group->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);

}
