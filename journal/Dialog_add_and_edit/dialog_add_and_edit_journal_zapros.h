#ifndef DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H
#define DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H

#include <QDialog>

namespace Ui {
class Dialog_add_and_edit_journal_zapros;
}

class Dialog_add_and_edit_journal_zapros : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_and_edit_journal_zapros(QWidget *parent = 0);
    ~Dialog_add_and_edit_journal_zapros();

private:
    Ui::Dialog_add_and_edit_journal_zapros *ui;
};

#endif // DIALOG_ADD_AND_EDIT_JOURNAL_ZAPROS_H
