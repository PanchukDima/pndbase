#ifndef DIALOG_REPORTS_H
#define DIALOG_REPORTS_H

#ifdef _WIN32
    #include <windows.h>
#endif

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
#include <QList>
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
    void make_report_vyt();
    QString make_report_vyt_sql(int, QString, QString);
    void make_report_1();
    void make_report_2();
    QString make_report_2_sql(QString);
    void make_report_7();
    QString make_report_7_sql(QString);
    void make_report_12();
    QString make_report_12_sql(QString);
    void make_report_17();
    QString make_report_17_sql(QString, QString);
    void make_report_19_forma_10();
    void make_report_24_forma_10_all_years();
    void make_report_15_suicid();
    void dynamic_invalid();
    void clicked_list();
    void start_generate();
    QString string_table_report(int);

};

#endif // DIALOG_REPORTS_H
