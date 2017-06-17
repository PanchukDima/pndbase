#ifndef DIALOG_ZAPROS_H
#define DIALOG_ZAPROS_H

#include <QDialog>
#include "journal/Dialog_add_and_edit/dialog_add_and_edit_journal_zapros.h"
#include "models/journal/model_journal_zapros.h"
#include <QSqlTableModel>

namespace Ui {
class Dialog_zapros;
}

class Dialog_zapros : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_zapros(QWidget *parent = 0);
    ~Dialog_zapros();
    model_journal_zapros *model;

private:
    Ui::Dialog_zapros *ui;
public slots:
    void add_zapros();
    void edit_zapros();
    void del_zapros();
    void load_table_param();


};

#endif // DIALOG_ZAPROS_H
