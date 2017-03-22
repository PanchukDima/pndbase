#ifndef DIALOG_ADD_DYNAMIC_VIEW_H
#define DIALOG_ADD_DYNAMIC_VIEW_H

#include <QDialog>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class Dialog_add_dynamic_view;
}

class Dialog_add_dynamic_view : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_dynamic_view(QWidget *parent = 0);
    ~Dialog_add_dynamic_view();
    QString global_id;
    QString global_id_row;
    int global_param;
    int global_type;
    QString global_staff;
    QString dynamic_type;
    QString help_type;
    QString block_message;
    bool block_state;
    QString dianose_id_patient;


private:
    Ui::Dialog_add_dynamic_view *ui;
public slots:
    void active_radio_button(bool);
    void active_radio_button_k(bool);
    void active_radio_button_lp(bool);
    void active_adn_check(bool);
    void active_apl_check(bool);
    void send_data();
    void setParam(int,QString,QString);
    void getData();
    void global_block();
    bool check_data(int);
    void load_diagnos_patient();
    void load_info_dynamic_view();
    QString some_d();
    void show_hide_close_date();

};

#endif // DIALOG_ADD_DYNAMIC_VIEW_H
