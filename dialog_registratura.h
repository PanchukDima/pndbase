#ifndef DIALOG_REGISTRATURA_H
#define DIALOG_REGISTRATURA_H

#include <QDialog>

namespace Ui {
class Dialog_registratura;
}

class Dialog_registratura : public QDialog
{
    Q_OBJECT


public:
    explicit Dialog_registratura(QWidget *parent = 0);
    ~Dialog_registratura();

private:
    Ui::Dialog_registratura *ui;
public slots:

};

#endif // DIALOG_REGISTRATURA_H
