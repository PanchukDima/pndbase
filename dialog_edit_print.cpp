#include "dialog_edit_print.h"
#include "ui_dialog_edit_print.h"

Dialog_edit_print::Dialog_edit_print(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_edit_print)
{
    ui->setupUi(this);
}

Dialog_edit_print::~Dialog_edit_print()
{
    delete ui;
}
