#ifndef DIALOG_PREPARATE_H
#define DIALOG_PREPARATE_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QIcon>
#include <QDate>
#include <Objects/User/objects_app.h>



namespace Ui {
class Dialog_Preparate;
}

class Dialog_Preparate : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Preparate(QWidget *parent = 0);
    ~Dialog_Preparate();
    int global_param;
    QString global_id;

private:
    Ui::Dialog_Preparate *ui;
public slots:
    void setParam(int,QString);
    void get_sql();
    void load_sql();
    void load_library();

};

#endif // DIALOG_PREPARATE_H
