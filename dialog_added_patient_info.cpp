#include "dialog_added_patient_info.h"
#include "ui_dialog_added_patient_info.h"

dialog_added_patient_info::dialog_added_patient_info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_added_patient_info)
{
    ui->setupUi(this);
}

dialog_added_patient_info::~dialog_added_patient_info()
{
    delete ui;
}
