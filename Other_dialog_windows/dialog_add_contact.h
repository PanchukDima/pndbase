#ifndef DIALOG_ADD_CONTACT_H
#define DIALOG_ADD_CONTACT_H

#include <QDialog>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Dialog_add_contact;
}

class Dialog_add_contact : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_contact(QWidget *parent = 0);
    ~Dialog_add_contact();

private:
    Ui::Dialog_add_contact *ui;
public slots:
    void save_contact();
};

#endif // DIALOG_ADD_CONTACT_H
