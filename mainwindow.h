#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QtSql>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QContextMenuEvent>
#include <QThread>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QInputDialog>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCloseEvent>
#include <QItemSelectionModel>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSpacerItem>
//
#include "dialog_patient.h"
#include "dialog_settings_user.h"
#include "dialog_login.h"
#include "dialog_registratura.h"
#include "dialog_reports.h"
#include "dialog_add_dynamic_view.h"
#include "dialog_add_visits.h"
#include "dialog_preview_print.h"
#include "work_info_patient/diagnos/dialog_diagnos_patient.h"
#include "work_info_patient/diagnos/dialog_add_diagnos_and_dop_diagnos.h"
#include "dialog_hospitalization.h"
#include "dialog_copy_files_to_server.h"
#include "dialog_invalids_patient.h"
#include "dialog_find_filters.h"
#include "add_invalid_class.h"
#include "logs_save.h"
#include "appUI/Chat/dialog_chat.h"
#include "work_info_patient/suicide/dialog_suicide.h"
#include "work_info_patient/ood/dialog_ood.h"
#include "journal/dialog_rvk.h"
#include "work_info_patient/list_not_work/dialog_list_not_work.h"
#include "work_info_patient/dynamic_view/dialog_edit_other_dynamic_view.h"

//models other
#include <QFileSystemModel>


//Custom Models
#include "models/model_main_table.h"
#include "models/model_dynamic_view_patient.h"
#include "models/model_visits_control_patient.h"
#include "models/model_sved_gosp_patient.h"
#include "models/model_ivalid_patient.h"
#include "models/model_list_not_work.h"


//Network Library
#include <QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork>


//NoNStokLibrary

//threads
#include "new_changes_thread.h"

//Apps
#include <appUI/dialog_app.h>

//journals
    //RVK
#include <journal/Dialog_add_and_edit/dialog_add_and_edit_journal.h>
#include <journal/dialog_rvk.h>

//viewer
#include "work_info_patient/visitors/dialog_visiting_control.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString ipdatabase;
    QList <bool> rights_user;
    int font_size;
    int port;
    QLineEdit * str_find_patient;
    QLabel * valid_info;
    QString staff_id;
    int first_start;
    QTimer *timer;
    QString session_id;
    bool state_hide;
    QString area_append_user;
    QString staff_name;
    int select_row;

    bool fff;

    QUdpSocket * udpSocket;


protected:
      virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    model_main_table *model = nullptr;
    model_list_not_work *list_not_work_model = nullptr;
    QSqlRelationalTableModel *model_ood = nullptr;
    QSqlRelationalTableModel *model_suicide = nullptr;
    model_dynamic_view_patient *model_dynamic_view = nullptr;
    model_ivalid_patient *model_invalid = nullptr;
    QSqlRelationalTableModel *model_preparate = nullptr;
    model_visits_control_patient *model_visits_control = nullptr;
    model_sved_gosp_patient *model_sved_gosp = nullptr;
    QFileSystemModel *filemodel;
    QTimer * timer_who_is_online;
    QString selected_id;


public slots:
    //настройки моделей таблиц
    void set_table_param();

    //настройки приложения

    void load_translator();

    //получаем настройки из базы данных
    void load_settings_from_sql();
    void load_area_list();
    void load_rights_user();
    void close_old_session();
    void start_new_session();
    void load_settings_program();
    void load_count_visiting();
    void start_udp_server();

    void settings_ui();
    void settings_user();
    void hide_other_info();
    void open_find_filter();
    void settings_maintoolbar();

    void added_info_patient();
    void edit_info_patient();
    void view_info_patient();
    void del_info_patient();
    void show_document_patient();

    void open_win_settings();
    void load_main_table();
    void clear_main_table();

    //Остальные таблицы по разным функциям
    void load_list_not_work_model();
    void load_model_dynamic_view();
    void load_model_visits_control();
    void load_model_sved_gosp();
    void load_diagnos_table();
    void load_files_patient();

    void load_invalid_table();
    void load_suicide_table();
    void load_ood_table();
    //...
    void clear_diagnos_table();

    void clear_invalid_table();
    void find_patients();
    void load_all_info();
    void load_other_info();

    //контекстное меню
    void context_menu_dynamic_view(QPoint);
    void context_menu_visits_control(QPoint);
    void context_menu_main_table(QPoint);
    void context_menu_diagnos_table(QPoint);
    void context_menu_hospitalization_table(QPoint);
    void context_menu_invalid_table(QPoint);
    void context_menu_suicide_table(QPoint);
    void context_menu_ood_table(QPoint);
    void context_menu_list_not_work_table(QPoint);
    void context_menu_files_list(QPoint);

    void add_dynamic_view();
    void close_dynamic_view();
    void edit_dynamic_view();
    void del_dynamic_view();
    void dialog_add_file_to_patient();

    void add_visit();
    void add_today_visit();
    void add_yesterday_visits();
    void add_day_before_yesterday_visits();
    void edit_visit();
    void del_visit();

    void add_diagnos_patient();
    void add_child_diagnost_patient();
    void edit_diagnos_patient();
    void del_diagnos_patient();

    void add_hospitalization();
    void edit_hospitalization();
    void del_hospitalization();

    void add_invalid();

    void del_invalid();
    void edit_invalid();

    void add_suicide();
    void edit_suicide();
    void del_suicide();

    void add_ood();
    void edit_ood();
    void del_ood();

    void add_list_not_work();
    void edit_list_not_work();
    void del_list_not_work();

    //Отчеты и прочее
    void gen_report_1();
    void gen_report_9(); //Динамика инвалидности
    void gen_report_19();//форма 10
    void gen_report_24();

    void print_medcard();
    void set_status_arhive();
    void changes_new(QString status);
    void added_files_patient();


    void select_row_on_table_to_medcard(QString, QString ,int);




    //threads
    void thread_new_changes();
    void test_curent_version();
    void rights_update();

    void open_app();
    void open_journal_rvk();

    //Notification
    void notification_server(QString);

    //Remote Control
    void loadCommand();
    void I_Online();

    //Journal
    void add_zapis_rvk();
    void add_zapis_query();
    void find_zapis_rvk();
    void find_zapis_query();

    void view_visiting();
    void show_visiting_today();

    void sort_gosp(int);


};

#endif // MAINWINDOW_H
