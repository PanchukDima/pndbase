#ifndef DIALOG_PATIENT_H
#define DIALOG_PATIENT_H

#ifdef _WIN32
    #include <windows.h>
#endif

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QIcon>
#include <QInputDialog>
#include <QSqlRelationalTableModel>
#include <QKeyEvent>

#include <QSqlError>
#include <QPrintDialog>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocumentWriter>

#include "dialog_edit_print.h"
#include "ui/Print/dialog_print_preview.h"
#include "Objects/User/objects_app.h"
#include <appUI/DocumentEditor/window_document_editor.h>


namespace Ui {
class Dialog_patient;
}

class Dialog_patient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_patient(QWidget *parent = 0);
    ~Dialog_patient();

    QSqlDatabase dbsql;
    int global_param;
    int global_id;
    int param_comments;
    QString edit_comments_id;
    QString global_id_str;
    QString global_staff_id;
    QString why_remove_str;
    bool text_change;
    QString id_text_document;
    QSqlTableModel * model_comments;
    QSqlRelationalTableModel * model_uchet;



private:
    Ui::Dialog_patient *ui;

protected:
    void keyPressEvent(QKeyEvent * e);

public slots:
    void setParam(int ,int, bool);
    void get_data_sql(int);
    void apply_send_data_sql();
    void put_all_settings();

    void ok_send_data_sql();
    void button_cancel();
    void add_street();
    void set_default_color_lineedit_fname();
    void set_default_color_lineedit_name();
    void set_default_color_lineedit_mname();
    void set_default_color_lineedit_serial_passport();
    void set_default_color_lineedit_number_passport();
    //void set_default_color_lineedit_house();
    void set_default_color_combox_sex();
    void set_default_color_combox_area();
    void set_default_color_combox_area_street();
    void check_data();
    void change_state_ds_end(bool);
    QString const validate_date(QString);
    void point_add_birtday(QString);
    void point_add_ds_start(QString);
    void point_add_ds_stop(QString);
    //Учет
    void add_histry();
    void table_uchet_history();
    void del_uchet();

    void edit_date_create_medcard();

    //Редактор текста


    void select_changes_text_document(QTextCharFormat);
    void change_combox_document();
    void load_documents();
    void select_document_on_table();


    void create_document();
    void delete_document();


    //void select_text_document();
    void open_new_document(QString);

    //comments
    void add_comments();
    void load_comments();
    void edit_comments();
    void del_comments();
    void context_menu_comments(QPoint);
    void copy_text_comments();

    void show_dialog_add_patient();

    void apply_rights();





};

#endif // DIALOG_PATIENT_H
