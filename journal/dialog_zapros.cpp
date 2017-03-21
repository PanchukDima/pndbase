#include "dialog_zapros.h"
#include "ui_dialog_zapros.h"

Dialog_zapros::Dialog_zapros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_zapros)
{
    ui->setupUi(this);
}

Dialog_zapros::~Dialog_zapros()
{
    delete ui;
}
