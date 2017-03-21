#ifndef DIALOG_ADD_AND_EDIT_JOURNAL_H
#define DIALOG_ADD_AND_EDIT_JOURNAL_H

#include <QDialog>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include "Other_dialog_windows/dialog_fast_find_patient_id.h"
#include "Other_dialog_windows/dialog_diagnos_selected_patient.h"
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Dialog_add_and_edit_journal;
}

class Dialog_add_and_edit_journal : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_and_edit_journal(QWidget *parent = 0);
    ~Dialog_add_and_edit_journal();
    QString id_patient;
    QString id_diagnos;
    QString name_diagnos;
    QString code_diagnos;
    QString street_id;
    QString house;
    QString building;
    QString flat;
    QString edit_id;
    QString id;

private:
    Ui::Dialog_add_and_edit_journal *ui;
public slots:
    void open_dialog_select_patient();
    void open_dialog_select_diagnos();
    void init_find_patient();
    void init_other_data();
    void input_signal(QString);
    void get_sql();
    void setParam(int, QString);
    void update_sql();
    void load_sql();
    void load_in_journal();


};

#endif // DIALOG_ADD_AND_EDIT_JOURNAL_H
