#ifndef DIALOG_FIND_FILTERS_H
#define DIALOG_FIND_FILTERS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QDebug>
#include <QTableView>
#include <QSqlQuery>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QSqlError>
#include <QWebView>

#include "models/model_find_table.h"
#include "models/model_dynamic_view.h"
#include "models/model_diagnos_filter.h"
#include "models/filters/model_filter_invalid.h"
#include "Objects/User/objects_app.h"
#include "dialog_patient.h"

namespace Ui {
class Dialog_find_filters;
}

class Dialog_find_filters : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_find_filters(QWidget *parent = 0);
    ~Dialog_find_filters();
    bool state_widget_finds;
    model_dynamic_view * model_dynamic;
    model_filter_invalid * model_invalid;
signals:
    void diagnos(QString, QString, int);
    void dynamic_view(QString, QString, int);


private:
    Ui::Dialog_find_filters *ui;

public slots:
    void send_result();
    void load_table(QString);
    void find_widget();
    void load_list();
    void SaveAsCSV();
    void Print_result();
    //Sql model Table
    void find_filter_diagnos();
    void find_filter_dynamic_view();
    void find_filter_invalid();


    void changes_code_diagnos(int);
    void changes_name_diagnos(int);
    void hidden_list_diagnos(int);
    void hidden_list_invalid(int);
    void context_menu_main_filter(QPoint);
    void context_menu_diagnos_filter(QPoint);
    void context_menu_dynamic_view_filter(QPoint);
    void view_patient_info();
    void edit_patient_info();
    void del_patient();
    void show_hide_area_list();
    void checked_all_area_in_list();
    void show_diagnos_in_main_table();
    void show_dynamic_view_in_main_table();
    void show_main_filter_in_main_table();

    QString load_isCheked_list_area();

};

#endif // DIALOG_FIND_FILTERS_H
