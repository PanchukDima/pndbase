#ifndef DIALOG_OOD_H
#define DIALOG_OOD_H

#include <QDialog>
#include <QtSql>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class Dialog_ood;
}

class Dialog_ood : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ood(QWidget *parent = 0);
    ~Dialog_ood();
    int global_param;
    QString global_id;
    QString global_staff_id;

private:
    Ui::Dialog_ood *ui;
public slots:
    void setParam(int,QString,QString);
    void load_data();
    void send_data();

};

#endif // DIALOG_OOD_H
