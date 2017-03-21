#ifndef DIALOG_SUICIDE_H
#define DIALOG_SUICIDE_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QIcon>
#include <QDate>

namespace Ui {
class Dialog_suicide;
}

class Dialog_suicide : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_suicide(QWidget *parent = 0);
    ~Dialog_suicide();
    QString global_id;
    QString global_staff_id;
    int global_param;

private:
    Ui::Dialog_suicide *ui;
public slots:
    void setParam(int param, QString id, QString staff_id);
    void send_data();
    void load_data();
    void load_lists();
};

#endif // DIALOG_SUICIDE_H
