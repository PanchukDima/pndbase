#include "dialog_emsr_find.h"
#include "ui_dialog_emsr_find.h"

Dialog_emsr_find::Dialog_emsr_find(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_emsr_find)
{
    ui->setupUi(this);
    connect(ui->pushButton_find,SIGNAL(clicked()),SLOT(load_emsrn()));
}

Dialog_emsr_find::~Dialog_emsr_find()
{
    delete ui;
}
void Dialog_emsr_find::setParam(int param,QString id)
{
    g_id = id;
    g_param = param;
    switch (param) {
    case 0:

        break;
    case 1:
        load_pnd();
        break;
    }
}
void Dialog_emsr_find::load_pnd()
{

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery *query = new QSqlQuery(db);
    QStringList pnd_data;
    if(db.open())
    {
        query->exec("SELECT medcard.serial_snils, medcard.polis_serial, medcard.polis_number, patient.fname, patient.name, patient.mname, medcard.sex, medcard.birthday,patient.serial_passport, patient.number_passport, patient.vidan,  concat(address_patient.index_adrr, street.name, CASE WHEN address_patient.building ='' THEN ''ELSE ' корп.'END , address_patient.building, ' д.',address_patient.home, ' кв.',address_patient.flat) AS Street, address_patient.telefon FROM test.medcard, test.patient, test.address_patient, test.street WHERE patient.id = medcard.patient_id AND address_patient.medcard_id = medcard.id AND street.id = address_patient.street_id AND medcard.id = "+g_id);
        if (query->lastError().isValid())
        {
            qDebug()<<query->lastError();
        }
        while (query->next())
        {
            pnd_data.append(query->value(0).toString());
            pnd_data.append(query->value(1).toString());
            pnd_data.append(query->value(2).toString());
            pnd_data.append(query->value(3).toString());
            pnd_data.append(query->value(4).toString());
            pnd_data.append(query->value(5).toString());
            pnd_data.append(query->value(6).toString());
            pnd_data.append(query->value(7).toString());
            pnd_data.append("");
            pnd_data.append("");
            pnd_data.append(query->value(8).toString());
            pnd_data.append(query->value(9).toString());
            pnd_data.append(query->value(10).toString());
            pnd_data.append(query->value(11).toString());
            pnd_data.append(query->value(12).toString());
            check_snils(query->value(0).toString());
        }
        ui->listWidget_pnd->addItems(pnd_data);

    }
}
void Dialog_emsr_find::load_emsrn()
{
    QSqlDatabase db = QSqlDatabase::database("emsrn");
    QSqlQuery *query = new QSqlQuery(db);
    QString snils = ui->lineEdit_snils->text();
    QStringList emsrn_data;
    if(db.open())
    {
        query->exec("SELECT [StrahNum],[PolicySer],[PolicyNum],[Fam],[Im],[Ot],[Sex],[BirthDate] ,[DocType],[DocName],[DocSer],[DocNum],[DocDate],[DocOrgName],[RegAdress],[FactAdress],[Tel],[PersonID_ESRN],[PersonID_EMSRN],[TMPR].[FMPINF] FROM [EMSRN_Load].[dbo].[Federal_Persons],[EMSRN_Load].[dbo].[TMPR]  WHERE [TMPR].[FMPIDN] = [Federal_Persons].[PersonID_EMSRN] AND StrahNum = '"+snils+"'");
        while (query->next())
        {
           emsrn_data.append(query->value(0).toString());
           emsrn_data.append(query->value(1).toString());
           emsrn_data.append(query->value(2).toString());
           emsrn_data.append(query->value(3).toString());
           emsrn_data.append(query->value(4).toString());
           emsrn_data.append(query->value(5).toString());
           emsrn_data.append(query->value(6).toString());
           emsrn_data.append(query->value(7).toString());
           emsrn_data.append(query->value(8).toString());
           emsrn_data.append(query->value(9).toString());
           emsrn_data.append(query->value(10).toString());
           emsrn_data.append(query->value(11).toString());
           emsrn_data.append(query->value(12).toString());
           emsrn_data.append(query->value(13).toString());
           emsrn_data.append(query->value(14).toString());
           emsrn_data.append(query->value(15).toString());
           emsrn_data.append(query->value(16).toString());
           if(query->value(19).toInt()<1073700000)
           {
              ui->lineEdit_emsrn_status->setText("Пока не известно");
           }
           else
           {
               ui->lineEdit_emsrn_status->setText("Смерть");
           }
        }
        ui->listWidget_emsrn->addItems(emsrn_data);
    }
}
void Dialog_emsr_find::check_snils(QString snils)
{
    QStringList parse_list = snils.split("-");
    if(parse_list.count()<1)
    {
        ui->lineEdit_snils->setText(snils);
    }
    else
    {
        ui->lineEdit_snils->setText(parse_list.join(""));
    }
}
void Dialog_emsr_find::get_info()
{

}
