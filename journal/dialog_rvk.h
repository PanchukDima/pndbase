#ifndef DIALOG_RVK_H
#define DIALOG_RVK_H

#include <QDialog>
#include "Dialog_add_and_edit/dialog_add_and_edit_journal.h"
#include <QMessageBox>

namespace Ui {
class Dialog_RVK;
}

class Dialog_RVK : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_RVK(QWidget *parent = 0);
    ~Dialog_RVK();
    QSqlTableModel *model = nullptr;
    QString id_patient;

private:
    Ui::Dialog_RVK *ui;
public slots:
    void add_push_button();
    void edit_push_button();
    void del_push_button();
    void input_signal(QString);
    void find_journal(int);
    void push_button_ok();
};

#endif // DIALOG_RVK_H
