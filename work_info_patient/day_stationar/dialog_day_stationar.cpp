#include "dialog_day_stationar.h"
#include "ui_dialog_day_stationar.h"

Dialog_day_stationar::Dialog_day_stationar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_day_stationar)
{
    ui->setupUi(this);
}

Dialog_day_stationar::~Dialog_day_stationar()
{
    delete ui;
}
