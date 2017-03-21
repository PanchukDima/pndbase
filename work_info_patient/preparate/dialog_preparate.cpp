#include "dialog_preparate.h"
#include "ui_dialog_preparate.h"

Dialog_Preparate::Dialog_Preparate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Preparate)
{
    ui->setupUi(this);
}

Dialog_Preparate::~Dialog_Preparate()
{
    delete ui;
}
