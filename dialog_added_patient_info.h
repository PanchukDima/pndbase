#ifndef DIALOG_ADDED_PATIENT_INFO_H
#define DIALOG_ADDED_PATIENT_INFO_H

#include <QDialog>

namespace Ui {
class dialog_added_patient_info;
}

class dialog_added_patient_info : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_added_patient_info(QWidget *parent = 0);
    ~dialog_added_patient_info();

private:
    Ui::dialog_added_patient_info *ui;
};

#endif // DIALOG_ADDED_PATIENT_INFO_H
