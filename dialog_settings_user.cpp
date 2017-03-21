#include "dialog_settings_user.h"
#include "ui_dialog_settings_user.h"

Dialog_settings_user::Dialog_settings_user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings_user)
{
    ui->setupUi(this);
    this->setWindowTitle("Настройки программы");
    load_settings();
    connect(ui->pushButton_apply,SIGNAL(clicked()),SLOT(apply_settings()));
    connect(ui->pushButton_default_settings_font,SIGNAL(clicked()),SLOT(default_settings()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked()),SLOT(push_ok()));
    connect(ui->pushButton_set_path_blanks,SIGNAL(clicked(bool)),SLOT(set_updServ_path_blanks_func()));
    connect(ui->pushButton_set_path_local_blanks,SIGNAL(clicked(bool)),SLOT(set_path_blanks_func()));
    connect(ui->toolButton_path_files_patient,SIGNAL(clicked(bool)),SLOT(set_path_files_patient()));
}

Dialog_settings_user::~Dialog_settings_user()
{
    delete ui;
}
void Dialog_settings_user::load_settings()
{
    ui->comboBox_format_list->addItem("A0",0);
    ui->comboBox_format_list->addItem("A1",1);
    ui->comboBox_format_list->addItem("A2",2);
    ui->comboBox_format_list->addItem("A3",3);
    ui->comboBox_format_list->addItem("A4",4);
    ui->comboBox_format_list->addItem("A5",5);
    ui->comboBox_format_list->addItem("A6",6);

    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    QString ip_addr = settings->value("ipdatabase").toString();
    QString port = settings->value("portdatabase").toString();
    QString sizefonttable = settings->value("sizefonttexttable").toString();
    QString version = settings->value("CurrentVersion").toString();
    QString path_UpdServ_blanks = settings->value("path_UpdServ_blanks").toString();
    QString path_blanks = settings->value("path_blanks").toString();
    QString path_files_patient = settings->value("path_files_patient").toString();
    QString name_ych = settings->value("name_ych").toString();
    QString adress_ych = settings->value("adress_ych").toString();
    QString ogrn_ych = settings->value("ogrn_ych").toString();
    QString director_name = settings->value("director_name").toString();
    int list_format = settings->value("Document/list_format").toInt();
    int MarL = settings->value("Document/MarL").toInt();
    int MarR = settings->value("Document/MarR").toInt();
    int MarB = settings->value("Document/MarB").toInt();
    int MarT = settings->value("Document/MarT").toInt();
    bool sign_type = settings->value("type_sign").toBool();
    bool sys_user = settings->value("sys_user_type").toBool();
    bool l_update = settings->value("ProgramUpdate/l_auto_update").toBool();

    ui->lineEdit_ip_addr->setText(ip_addr);
    ui->lineEdit_port->setText(port);
    ui->lineEdit_size_font_text_setting->setText(sizefonttable);
    //ui->label_CurrentVersion->setText(version);
    ui->lineEdit_path_blanks->setText(path_UpdServ_blanks);
    ui->lineEdit_path_local_blanks->setText(path_blanks);
    ui->lineEdit_path_files_patient->setText(path_files_patient);
    ui->lineEdit_name_ych->setText(name_ych);
    ui->lineEdit_adress_ych->setText(adress_ych);
    ui->lineEdit_OGRN_ych->setText(ogrn_ych);
    ui->lineEdit_director_name->setText(director_name);
    qDebug()<<list_format;
    ui->comboBox_format_list->setCurrentIndex(ui->comboBox_format_list->findData(list_format));
    ui->spinBox_MarL->setValue(MarL);
    ui->spinBox_MarR->setValue(MarR);
    ui->spinBox_MarB->setValue(MarB);
    ui->spinBox_MarT->setValue(MarT);

    if(sign_type)
    {
        ui->checkBox_auth_bd->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->checkBox_auth_bd->setCheckState(Qt::Checked);
    }
    if(sys_user)
    {
        ui->checkBox_sys_user_name->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_sys_user_name->setCheckState(Qt::Unchecked);
    }
    if(l_update)
    {
        ui->checkBox_l_Update->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_l_Update->setCheckState(Qt::Unchecked);
    }


}

void Dialog_settings_user::apply_settings()
{
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    settings->setValue("ipdatabase", ui->lineEdit_ip_addr->text());
    settings->setValue("portdatabase", ui->lineEdit_port->text());
    settings->setValue("sizefonttexttable", ui->lineEdit_size_font_text_setting->text());
    settings->setValue("path_UpdServ_blanks" ,ui->lineEdit_path_blanks->text());
    settings->setValue("path_blanks" ,ui->lineEdit_path_local_blanks->text());
    settings->setValue("path_files_patient", ui->lineEdit_path_files_patient->text());
    settings->setValue("name_ych", ui->lineEdit_name_ych->text());
    settings->setValue("adress_ych", ui->lineEdit_adress_ych->text());
    settings->setValue("ogrn_ych", ui->lineEdit_OGRN_ych->text());
    settings->setValue("director_name", ui->lineEdit_director_name->text());
    settings->setValue("sys_user_type", ui->checkBox_sys_user_name->isChecked());
    settings->setValue("type_sign", !ui->checkBox_sys_user_name->isChecked());
    settings->setValue("Document/list_format", ui->comboBox_format_list->currentData().toInt());
    settings->setValue("Document/MarL",ui->spinBox_MarL->value());
    settings->setValue("Document/MarR",ui->spinBox_MarR->value());
    settings->setValue("Document/MarB",ui->spinBox_MarB->value());
    settings->setValue("Document/MarT",ui->spinBox_MarT->value());
    settings->setValue("ProgramUpdate/l_auto_update",ui->checkBox_l_Update->isChecked());
}
void Dialog_settings_user::default_settings()
{
    ui->lineEdit_size_font_text_setting->setText("8");
}
void Dialog_settings_user::push_ok()
{
    apply_settings();
    Dialog_settings_user::accept();
}
void Dialog_settings_user::set_path_blanks_func()
{
    QString file_name=ui->lineEdit_path_local_blanks->text();
    file_name = QFileDialog::getExistingDirectory(this, tr("Путь к бланкам"), file_name, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_path_local_blanks->setText(file_name);
}
void Dialog_settings_user::set_updServ_path_blanks_func()
{
    QString file_name=ui->lineEdit_path_blanks->text();
    file_name = QFileDialog::getExistingDirectory(this, tr("Путь к сетевым бланкам"), file_name, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_path_blanks->setText(file_name);
}
void Dialog_settings_user::set_path_files_patient()
{
    QString file_name=ui->lineEdit_path_files_patient->text();
    file_name = QFileDialog::getExistingDirectory(this, tr("Путь к файлам пациента"), file_name, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_path_files_patient->setText(file_name);
}
