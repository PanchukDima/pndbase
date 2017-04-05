#include "dialog_page_settings.h"
#include "ui_dialog_page_settings.h"

Dialog_page_settings::Dialog_page_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_page_settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Параметры страницы");
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(save_settings()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    ui->comboBox_page_format->addItem("A0",0);
    ui->comboBox_page_format->addItem("A1",1);
    ui->comboBox_page_format->addItem("A2",2);
    ui->comboBox_page_format->addItem("A3",3);
    ui->comboBox_page_format->addItem("A4",4);
    ui->comboBox_page_format->addItem("A5",5);
    ui->comboBox_page_format->addItem("A6",6);
    load_settings();
}

Dialog_page_settings::~Dialog_page_settings()
{
    delete ui;
}
void Dialog_page_settings::load_settings()
{
    Objects_app obj;
    QSettings *settings = new QSettings(obj.path_settings,QSettings::IniFormat);
    int list_format = settings->value("Document/list_format").toInt();
    int MarL = settings->value("Document/MarL").toInt();
    int MarR = settings->value("Document/MarR").toInt();
    int MarB = settings->value("Document/MarB").toInt();
    int MarT = settings->value("Document/MarT").toInt();
    ui->comboBox_page_format->setCurrentIndex(ui->comboBox_page_format->findData(list_format));
    ui->spinBox_MarL->setValue(MarL);
    ui->spinBox_MarR->setValue(MarR);
    ui->spinBox_MarB->setValue(MarB);
    ui->spinBox_MarT->setValue(MarT);

}
void Dialog_page_settings::save_settings()
{
    Objects_app obj;
    QSettings *settings = new QSettings(obj.path_settings,QSettings::IniFormat);
    settings->setValue("Document/list_format", ui->comboBox_page_format->currentData().toInt());
    settings->setValue("Document/MarL",ui->spinBox_MarL->value());
    settings->setValue("Document/MarR",ui->spinBox_MarR->value());
    settings->setValue("Document/MarB",ui->spinBox_MarB->value());
    settings->setValue("Document/MarT",ui->spinBox_MarT->value());
    Dialog_page_settings::accept();
}
