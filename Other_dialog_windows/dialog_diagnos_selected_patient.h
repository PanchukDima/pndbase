#ifndef DIALOG_DIAGNOS_SELECTED_PATIENT_H
#define DIALOG_DIAGNOS_SELECTED_PATIENT_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDebug>
#include "Objects/User/objects_app.h"

namespace Ui {
class Dialog_diagnos_selected_patient;
}

class Dialog_diagnos_selected_patient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_diagnos_selected_patient(QWidget *parent = 0);
    ~Dialog_diagnos_selected_patient();
    QString medcard_id;
    QString diagnos_id;
    QString diagnos_name;
    QString diagnos_code;

private:
    Ui::Dialog_diagnos_selected_patient *ui;
public slots:
    void select_diagnos_patient();
    void double_click_table();
    void setParam(QString);
};

#endif // DIALOG_DIAGNOS_SELECTED_PATIENT_H
