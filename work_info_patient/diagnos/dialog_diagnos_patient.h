#ifndef DIALOG_DIAGNOS_PATIENT_H
#define DIALOG_DIAGNOS_PATIENT_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QIcon>
#include <QDate>
#include "Objects/User/objects_app.h"

namespace Ui {
class Dialog_diagnos_patient;
}

class Dialog_diagnos_patient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_diagnos_patient(QWidget *parent = 0);
    ~Dialog_diagnos_patient();
    int global_param;
    QString global_id;
    QString global_staff_id;

private:
    Ui::Dialog_diagnos_patient *ui;
public slots:
    void setParam(int param, QString id, QString staff_id);
    void get_data();
    void send_data();
    void load_allow_date();
    void change_code_diagnos(int);
    void change_description_diagnos(int);
};


#endif // DIALOG_DIAGNOS_PATIENT_H
