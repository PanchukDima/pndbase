#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include <QDialog>
#include <QFile>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlError>
#include <QMessageBox>
#include <QTime>
#include <QKeyEvent>
#include <QProcess>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QPrinterInfo>
#include <QHostInfo>
#include <QDesktopWidget>


#include <Objects/User/objects_app.h>

#include <dialog_settings_user.h>


namespace Ui {
class Dialog_login;
}

class Dialog_login : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_login(QWidget *parent = 0);
    ~Dialog_login();
    QString staff_id;
    bool type_sign;



private:
    Ui::Dialog_login *ui;
public slots:
    void login_db();
    void quit_app();
    void clear_label_status();
    void open_settings();
    void update_launcher();
    void correct_time();
    void beta_antivirus();


};

#endif // DIALOG_LOGIN_H
