#ifndef DIALOG_EMSR_FIND_H
#define DIALOG_EMSR_FIND_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>

namespace Ui {
class Dialog_emsr_find;
}

class Dialog_emsr_find : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_emsr_find(QWidget *parent = 0);
    ~Dialog_emsr_find();
    QString g_id;
    int g_param;

private:
    Ui::Dialog_emsr_find *ui;
public slots:
    void setParam(int,QString);
    void load_emsrn();
    void load_pnd();
    void get_info();
    void check_snils(QString);
};

#endif // DIALOG_EMSR_FIND_H
