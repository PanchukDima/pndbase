#include "dialog_add_and_edit_journal_zapros.h"
#include "ui_dialog_add_and_edit_journal_zapros.h"

Dialog_add_and_edit_journal_zapros::Dialog_add_and_edit_journal_zapros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_and_edit_journal_zapros)
{
    ui->setupUi(this);
}

Dialog_add_and_edit_journal_zapros::~Dialog_add_and_edit_journal_zapros()
{
    delete ui;
}
