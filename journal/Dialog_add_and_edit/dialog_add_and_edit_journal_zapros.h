#ifndef DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H
#define DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H

#include <QDialog>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Other_dialog_windows/dialog_add_contact.h"

namespace Ui {
class Dialog_add_and_edit_journal_zapros;
}

class Dialog_add_and_edit_journal_zapros : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_and_edit_journal_zapros(QWidget *parent = 0);
    ~Dialog_add_and_edit_journal_zapros();
    int global_param;
    QString global_id;

private:
    Ui::Dialog_add_and_edit_journal_zapros *ui;
public slots:
    void load_library();
    void get_sql();
    void load_sql();
    void setParam(int);
    void edit(QString);
    void open_add_contact();
    void load_library_contact();
    void code_diagnos_edit();
    void name_diagnos_edit();
};

#endif // DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H
