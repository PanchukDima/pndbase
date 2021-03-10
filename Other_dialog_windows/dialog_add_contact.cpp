#include "dialog_add_contact.h"
#include "ui_dialog_add_contact.h"

Dialog_add_contact::Dialog_add_contact(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_contact)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(save_contact()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_add_contact::~Dialog_add_contact()
{
    delete ui;
}
void Dialog_add_contact::save_contact()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("INSERT INTO library.contact_list(name) VALUES ('"+ui->lineEdit_name->text()+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            Dialog_add_contact::close();
        }
        else
        {
            Dialog_add_contact::accept();
        }
    }
}
