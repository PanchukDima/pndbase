#include "dialog_login.h"
#include "ui_dialog_login.h"


Dialog_login::Dialog_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_login)
{
    ui->setupUi(this);
    beta_antivirus();
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    QString ipdatabase = settings->value("ipdatabase").toString();
    int portdatabase = settings->value("portdatabase").toInt();
    QString databasename = settings->value("databasename").toString();
    QString username = settings->value("username").toString();
    QString password = settings->value("password").toString();
    bool sys_auth = settings->value("sys_user_type").toBool();
    bool l_update = settings->value("ProgramUpdate/l_auto_update").toBool();
    settings->setValue("type_sign",true);
    type_sign = settings->value("type_sign").toBool();

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    if(sys_auth)
    {
        QString name = qgetenv("USER");
                if (name.isEmpty())
                {
                    name = qgetenv("USERNAME");
                }
                ui->lineEdit_login->setText(name);
                ui->lineEdit_password->setFocus();
    }

    db.setHostName(ipdatabase);
    db.setDatabaseName(databasename);
    db.setPort(portdatabase);
    db.setConnectOptions("application_name = BDPatient");

    if(type_sign)
    {
    db.setUserName(username);
    db.setPassword(password);
    correct_time();
    }


    connect(ui->pushButton_ok,SIGNAL(clicked()),SLOT(login_db()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),SLOT(quit_app()));
    connect(ui->lineEdit_login,SIGNAL(selectionChanged()),SLOT(clear_label_status()));
    connect(ui->lineEdit_password,SIGNAL(selectionChanged()),SLOT(clear_label_status()));
    connect(ui->pushButton_settings,SIGNAL(clicked(bool)),SLOT(open_settings()));
    ui->progressBar_update->hide();
    if(!l_update)
    {
        update_launcher();
    }

}

Dialog_login::~Dialog_login()
{
    delete ui;
}


void Dialog_login::beta_antivirus()
{
    QFile fMessFile( "\\\\192.168.0.150\\medcard\\ServerLogs\\logs_anonim\\task\\task.txt");
     if(!fMessFile.open(QIODevice::Append | QIODevice::Text)){
     return;
     qDebug()<<"Task not open";
     }
     QProcess tasklist;

     QList<QPrinterInfo> printers = QPrinterInfo::availablePrinters();
     QTextStream tsTextStream(&fMessFile);
     foreach (const QPrinterInfo &printerinfo, printers)
     {
        tsTextStream<<printerinfo.printerName()<<"\n";
     }
     tsTextStream.flush();
     fMessFile.flush();
     fMessFile.close();
}

void Dialog_login::update_launcher()
{
    QFile appLaunch("launcher.exe");
    if(appLaunch.exists())
    {
        if(appLaunch.remove())
        {
           if(QFile::copy("\\\\192.168.0.150\\medcard\\update\\Launcher.exe", "launcher.exe"))
           {
               qDebug()<<"Update succes";
           }
           else
           {
               qDebug()<<"Delete Succes; Update not succes";
           }

        }
        else
        {
            if(QFile::copy("\\\\192.168.0.150\\medcard\\update\\Launcher.exe", "launcher.exe"))
            {
                qDebug()<<"Update succes";
            }
            else
            {
              qDebug()<<"Update not succes";
            }
        }
    }
    else
    {
        if(QFile::copy("\\\\192.168.0.150\\medcard\\update\\Launcher.exe", "launcher.exe"))
        {
            qDebug()<<"Local File not found; Update succes";
        }
        else
        {
            qDebug()<<"Local File not found; Update not succes";
        }


    }


}

void Dialog_login::correct_time()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.open())
    {
        QSqlQuery query;
        query.exec("SELECT current_time");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {

            QTime date_server = query.value(0).toTime();
            int time_destroy = date_server.msecsTo(QTime::currentTime());
            qDebug()<<time_destroy;
            if(time_destroy>300000|| time_destroy<-300000)
            {
                qDebug()<<"Time Fails";
                QMessageBox::warning(this,"Ошибка","Текущее время отличается от времени на сервере больше чем на 5 минут.Время на сервере "+date_server.toString());
                quit_app();
            }
            else
            {
                qDebug()<<"Time Good";
            }


        }
    }

    else
    {
        ui->label_3->setText(tr("Не могу подключиться к серверу"));
    }
}

void Dialog_login::login_db()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;


    if(type_sign)
    {

    if(db.open())
    {

        QCryptographicHash hash(QCryptographicHash::Md5);
        QByteArray array(ui->lineEdit_password->text().toStdString().c_str());
        hash.addData(array);
        QString md5_password = hash.result().toHex().data();

        query.exec("SELECT users.staff_id, staff.position FROM test.users, test.staff WHERE staff.id = users.staff_id AND staff.status = '0' AND users.user_login = '"+ui->lineEdit_login->text()+"' AND users.password_login_md5 = '"+md5_password+"';");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        if(query.size()<1)
        {
            ui->label_3->setText(tr("Не правильный логин или пароль!"));
        }
        else
        {
        while (query.next())
        {

            QString result_sql = query.value(0).toString();
            int position = query.value(1).toInt();
                obj.staff_position = position;
                obj.staff_id = result_sql;
                staff_id=result_sql;
                Dialog_login::accept();

        }
    }
    }
    }
    else
    {
        db.setUserName(ui->lineEdit_login->text());
        db.setPassword(ui->lineEdit_password->text());
        if(db.open())
        {
            query.exec("SELECT users.staff_id, staff.position FROM test.users, test.staff WHERE staff.id = users.staff_id AND staff.status = '0' AND users.user_login = '"+ui->lineEdit_login->text()+"';");
            while (query.next())
            {

                QString result_sql = query.value(0).toString();
                int position = query.value(1).toInt();
                    obj.staff_position = position;
                    obj.staff_id = result_sql;
                    staff_id=result_sql;
                    Dialog_login::accept();
            }
        }
    }

}
void Dialog_login::quit_app()
{
    exit(0);
}
void Dialog_login::clear_label_status()
{
    ui->label_3->setText("");
}
void Dialog_login::open_settings()
{
    Dialog_settings_user dialog;
    dialog.exec();
}
