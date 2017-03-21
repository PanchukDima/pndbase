#include "dialog_registratura.h"
#include "ui_dialog_registratura.h"

Dialog_registratura::Dialog_registratura(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_registratura)
{
    ui->setupUi(this);
}

Dialog_registratura::~Dialog_registratura()
{
    delete ui;
}

