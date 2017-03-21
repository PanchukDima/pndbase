#ifndef DIALOG_PREVIEW_PRINT_H
#define DIALOG_PREVIEW_PRINT_H

#include <QDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QFile>
#include <QtSql>
#include <QSettings>
#include <QSqlError>
#include <QMessageBox>
namespace Ui {
class Dialog_preview_print;
}

class Dialog_preview_print : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_preview_print(QWidget *parent = 0);
    ~Dialog_preview_print();
    QString global_id;
    QString global_staff_id;
    int global_param;

private:
    Ui::Dialog_preview_print *ui;
public slots:
    void print();
    void setParam(int,QString,QString);
    void export_word();

};

#endif // DIALOG_PREVIEW_PRINT_H
