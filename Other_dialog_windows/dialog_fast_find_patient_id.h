#ifndef DIALOG_FAST_FIND_PATIENT_ID_H
#define DIALOG_FAST_FIND_PATIENT_ID_H

#include <QDialog>
#include <QSqlTableModel>
#include "models/model_main_table.h"
#include "Objects/User/objects_app.h"
namespace Ui {
class Dialog_fast_find_patient_id;
}

class Dialog_fast_find_patient_id : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_fast_find_patient_id(QWidget *parent = 0);
    ~Dialog_fast_find_patient_id();
    QString id;

private:
    Ui::Dialog_fast_find_patient_id *ui;
    model_main_table *model = nullptr;

public slots:
    void find_patient();
    void double_click_patient();
};

#endif // DIALOG_FAST_FIND_PATIENT_ID_H
