#ifndef DIALOG_INVALIDS_PATIENT_H
#define DIALOG_INVALIDS_PATIENT_H

#include <QDialog>
#include <QDate>
#include <QSqlError>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include "Objects/User/objects_app.h"


namespace Ui {
class Dialog_invalids_patient;
}

class Dialog_invalids_patient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_invalids_patient(QWidget *parent = 0);
    ~Dialog_invalids_patient();
    QString global_id;
    QString global_staff_id;
    int global_param;

private:
    Ui::Dialog_invalids_patient *ui;
public slots:
    void setParam(int, QString, QString);
    void get_data();
    void send_data();
    void setting();
    void check_state_changes(int);
    QString const validate_date(QString);
    void point_add_date(QString);
    void point_add_date_preosved(QString);
};

#endif // DIALOG_INVALIDS_PATIENT_H
