#include "dialog_table_add.h"
#include "ui_dialog_table_add.h"

Dialog_table_add::Dialog_table_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_table_add)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(create_table()));
}

Dialog_table_add::~Dialog_table_add()
{

    delete ui;
}
void Dialog_table_add::create_table()
{
    x = ui->spinBox_row->value();
    y = ui->spinBox_collumn->value();
    emit table(x,y);
    Dialog_table_add::accept();
}
