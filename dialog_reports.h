#ifndef DIALOG_REPORTS_H
#define DIALOG_REPORTS_H

#include <QDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>
#include <QMessageBox>
#include <Objects/User/objects_app.h>
namespace Ui {
class Dialog_reports;
}

class Dialog_reports : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_reports(QWidget *parent = 0);
    ~Dialog_reports();
    int global_num;
    QString htm;

private:
    Ui::Dialog_reports *ui;
public slots:
    void gen_report(int);
    void open_excel();
    void print_doc();
    void load_report();
    void make_report_19_forma_10();
    void make_report_24_forma_10_all_years();
    void dynamic_invalid();
    void clicked_list();
    void start_generate();

};

#endif // DIALOG_REPORTS_H
