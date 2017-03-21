#ifndef DIALOG_ADD_DIAGNOS_AND_DOP_DIAGNOS_H
#define DIALOG_ADD_DIAGNOS_AND_DOP_DIAGNOS_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QIcon>
#include <QDate>
#include "Objects/User/objects_app.h"


namespace Ui {
class Dialog_add_diagnos_and_dop_diagnos;
}

class Dialog_add_diagnos_and_dop_diagnos : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_diagnos_and_dop_diagnos(QWidget *parent = 0);
    ~Dialog_add_diagnos_and_dop_diagnos();
    QString global_id;
    QString staff_add_id;
    QString diagnos_id_add;

private:
    Ui::Dialog_add_diagnos_and_dop_diagnos *ui;
public slots:
    void setParam(QString, QString);
    void load_allow_data();
    void add_main_diagnos();
    void add_dop_diagnos();
    void changes_diagnos_code_main(int);
    void changes_diagnos_code_dop(int);
    void changes_diagnos_name_main(int);
    void changes_diagnos_name_dop(int);
};

#endif // DIALOG_ADD_DIAGNOS_AND_DOP_DIAGNOS_H
