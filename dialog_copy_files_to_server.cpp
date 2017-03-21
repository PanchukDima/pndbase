#include "dialog_copy_files_to_server.h"
#include "ui_dialog_copy_files_to_server.h"

Dialog_copy_files_to_server::Dialog_copy_files_to_server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_copy_files_to_server)
{
    ui->setupUi(this);
    connect(ui->pushButton_add,SIGNAL(clicked(bool)),SLOT(files_send()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
}

Dialog_copy_files_to_server::~Dialog_copy_files_to_server()
{
    delete ui;
}
void Dialog_copy_files_to_server::setParam(QString id, QString staff)
{
    global_id = id;
    global_staff = staff;
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    path = settings->value("path_files_patient").toString();
    files_load();
}

void Dialog_copy_files_to_server::files_load()
{
    ui->listWidget_files->clear();
    QDir dir(path+"/"+global_id);

    if(!dir.exists())
    {
        dir.mkpath(".");
    }
    QStringList jpgs = dir.entryList(QStringList(), QDir::Files | QDir::NoDotAndDotDot);
    ui->listWidget_files->addItems(jpgs);
}
void Dialog_copy_files_to_server::files_send()
{

    QString fileName =QFileDialog::getOpenFileName(this, tr("Добавить файл"), tr("*.*"));
    QStringList str=fileName.split("/");
    QString path_to;
    path_to = path+"/"+global_id+"/"+str[str.count()-1];
    QFile::copy(fileName,path_to);
    files_load();

}
