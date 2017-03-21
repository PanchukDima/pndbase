#include "dialog_preview_print.h"
#include "ui_dialog_preview_print.h"

Dialog_preview_print::Dialog_preview_print(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_preview_print)
{
    ui->setupUi(this);
    connect(ui->pushButton_Print,SIGNAL(clicked(bool)),SLOT(print()));
    connect(ui->pushButton_word_export,SIGNAL(clicked(bool)),SLOT(export_word()));
    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked(bool)),SLOT(accept()));
}

Dialog_preview_print::~Dialog_preview_print()
{
    delete ui;
}
void Dialog_preview_print::setParam(int param, QString id, QString staff_id)
{
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    global_id = id;
    global_staff_id=staff_id;
    global_param = param;
    QString path_blanks;
    QString text_html;
    QByteArray text;
    QFile file;
    path_blanks.append(settings->value("path_blanks").toString()).append("/");
    //    QString control_char = path_blanks.at(0);
    //    if(control_char=="/")
    //    {
    //
    //    }
    //    else
    //    {
    //        path_blanks.append("file://").append(settings->value("path_blanks").toString()).append("/");
    //    }

    switch (param) {
    case 0:
        path_blanks.append("medcard.html");


        file.setFileName(path_blanks);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        text_html = text.data();
        query.exec("SELECT\
                   medcard.id,\
                   patient.fname,\
                   patient.name,\
                   patient.mname,\
                   medcard.serial_snils,\
                   medcard.birthday,\
                   medcard.sex, \
                   patient.serial_passport, \
                   patient.number_passport, \
                   address_patient.index_adrr, \
                   street.name, \
                   address_patient.building, \
                   address_patient.flat,\
                   address_patient.home,\
                   address_patient.telefon, \
                   medcard.job_place, \
                   area.name, \
                   medcard.group_lgot_preparat,\
                   medcard.district_id,\
                   medcard.polis_serial,\
                   medcard.polis_number,\
                   patient.vidan\
                   FROM \
                   test.area, \
                   test.address_patient, \
                   test.street,\
                   test.medcard, \
                   test.patient\
                   WHERE \
                   area.id = medcard.area_id AND\
                address_patient.medcard_id = medcard.id AND\
                street.id = address_patient.street_id AND\
                patient.id = medcard.patient_id AND medcard.id ="+global_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                while(query.next())
        {
                text_html.replace("$NUM_MEDCARD$", global_id);
                text_html.replace("$FIO$", query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString());
                text_html.replace("$SNILS$", query.value(4).toString());
                text_html.replace("$BIRTHDAY$", query.value(5).toDate().toString("dd.MM.yyyy"));
                text_html.replace("$PASSPORT$", query.value(7).toString()+" "+query.value(8).toString());
                text_html.replace("$INDEX_ADDR$",query.value(9).toString());
                text_html.replace("$STREET$", query.value(10).toString());
                text_html.replace("$HOUSE$",query.value(13).toString());
                text_html.replace("$FLAT$",query.value(12).toString());
                text_html.replace("$TELEFON$", query.value(14).toString());
                text_html.replace("$WORK_POST$", query.value(15).toString());
                text_html.replace("$AREA$", query.value(16).toString());
                text_html.replace("$GROUP_LGOT_PREPARAT$", query.value(17).toString());
                text_html.replace("$BUILDING$",query.value(11).toString());
                text_html.replace("$POLIS_SERIAL$",query.value(19).toString());
                text_html.replace("$POLIS_NUMBER$",query.value(20).toString());
                text_html.replace("$PASSPORT_VIDAN$",query.value(21).toString());
                if(query.value(18).toString()=="1")
        {
                text_html.replace("$DISTRICT$","Выборгский");
    }
                else if(query.value(18).toString()=="2")

        {
                text_html.replace("$DISTRICT$","Калининский");
    }
                if(query.value(6).toBool())
        {
                text_html.replace("$SEX$", "М");
    }
                else
        {
                text_html.replace("$SEX$", "Ж");
    }
    }

                
                ui->webView->setHtml(text_html);

        break;
    case 1:
        //preview alow
        break;
    }
}
void Dialog_preview_print::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        ui->webView->print(&printer);
    }
    delete dlg;
}
void Dialog_preview_print::export_word()
{

}
