#include "dialog_reports.h"
#include "ui_dialog_reports.h"

#ifdef _WIN32
#include <QAxObject>
#include <QAxWidget>
#endif

Dialog_reports::Dialog_reports(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_reports)
{
    ui->setupUi(this);
    load_report();

    connect(ui->pushButton_excel,SIGNAL(clicked()),SLOT(open_excel()));
    connect(ui->pushButton_print,SIGNAL(clicked()),SLOT(print_doc()));
    connect(ui->pushButton_gen_rep,SIGNAL(clicked(bool)),SLOT(start_generate()));

}

Dialog_reports::~Dialog_reports()
{
    delete ui;
}
void Dialog_reports::gen_report(int num)
{
    global_num = num;


}
void Dialog_reports::load_report()
{
    int year = QDate::currentDate().year();
    for(int i =0; i<=5; i++)
    {
        QString year_str;
        year_str.setNum(year-i);
        QListWidgetItem * item = new QListWidgetItem();
        item->setText(year_str);
        ui->listWidget_year->addItem(item);
    }
}

void Dialog_reports::open_excel()
{
#ifdef _WIN32
    QAxWidget excel("Excel.Application");
    excel.setProperty("Visible", true);
    QAxObject * workbooks = excel.querySubObject("WorkBooks");
    switch (global_num) {
    case 1:
        workbooks->dynamicCall("Open (const QString&)", QString("C:/tmp/test3.xlsx"));
        QAxObject *workbook = excel.querySubObject("ActiveWorkBook");
        QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);
        QAxObject * range = worksheet->querySubObject("Cells(1,1)");
        range->setProperty("Value", QVariant("Helloo Excel"));
        break;

    }
#endif


}
void Dialog_reports::print_doc()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        ui->webView->print(&printer);
    }
    delete dlg;
}
void Dialog_reports::make_report_19_forma_10()
{
    qDebug()<<"Start Generation forma 10";
    QMap<int, int> column_1;
    QMap<int, int> column_2;
    QMap<int, int> column_3;
    QMap<int, int> column_4;
    QMap<int, int> column_5;
    QMap<int, int> column_6;
    QMap<int, int> column_7;
    QMap<int, int> column_8;
    QMap<int, int> column_9;
    QMap<int, int> column_10;
    QMap<int, QString> column_name;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*)\
                   FROM test.diagnos_patient,\
                   test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
                   WHERE \
                   medcard.id = diagnos_patient.medcard_id AND\
                patient.id = medcard.patient_id AND\
                age_patients.id = medcard.id AND\
                uchet.medcard_id = medcard.id\
                AND patient.status in (99,100,101) AND\
                uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
                AND\
                patient.delete_row = 'f'\
                AND\
                medcard.sex = 'f' AND \
                (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false \
                group by group_diagnos.id order by group_diagnos.id ");//women (Забыл как по Русски пишется=)
                if(query.lastError().isValid())
        {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
                while(query.next())
        {
                column_2[query.value(0).toInt()] = query.value(2).toInt();
    }
}

if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            age_patients.date_part >= 0 AND age_patients.date_part <= 14 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_3[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            age_patients.date_part >= 15 AND age_patients.date_part <= 17 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_4[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id  ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id \
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            age_patients.date_part >= 18 AND age_patients.date_part <= 19 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_5[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id  ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            age_patients.date_part >= 20 AND age_patients.date_part <= 39 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_6[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id , group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            age_patients.date_part >= 40 AND age_patients.date_part <= 59 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_7[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            patient.status in (99,100,101) AND\
            uchet.medcard_id = medcard.id AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            patient.delete_row = 'f'\
            AND\
            age_patients.date_part >= 60 AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_8[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id , group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id AND\
            dynamic_view.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            patient.delete_row = 'f'	\
            AND\
            dynamic_view.group_disp_view in('1','2','3','4','5','6','7') AND dynamic_view.status = 'f' AND dynamic_view.delete_row = 'f'\
            AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_9[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id, group_diagnos.name, count(*)\
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view, test.uchet\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            uchet.medcard_id = medcard.id AND\
            dynamic_view.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            uchet.on_uchet > '2016-01-01'::date AND uchet.on_uchet < '2016-12-31'::date\
            AND\
            patient.delete_row = 'f'	\
            AND\
            dynamic_view.group_disp_view in('10') AND dynamic_view.status = 'f' AND dynamic_view.delete_row = 'f'\
            AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_10[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT id, name FROM test.group_diagnos;");
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    while(query.next())
    {
        column_name[query.value(0).toInt()] = query.value(1).toString();
    }
}

if(db.open())
{
    query.exec("SELECT group_diagnos.id, group_diagnos.name, count(*)\
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            dynamic_view.medcard_id = medcard.id\
            and\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND diagnos_patient.fixing_diagnos_date > '2016-01-01'::date AND diagnos_patient.fixing_diagnos_date <= '2016-12-31'::date AND diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_1[query.value(0).toInt()] = query.value(2).toInt();
}
}



qDebug()<<column_name;
htm.append("<table border='1'>");
htm.append("<tr><td>Код по МКБ-X(класс V,адаптированный для использованияв РФ</td><td>Номер строки</td><td>Всего</td><td>из них женщин</td><td>0-14 лет</td><td>15-17 лет</td><td>18-19 лет</td><td>20-39лет</td><td>40-59 лет</td><td>60 лет и выше</td><td>Диспансерные пациенты</td><td>консультативные пациенты</td>");
for(int i=1; i<25; i++)
{
    QString collumn_1;
    QString collumn_2;
    QString collumn_3;
    QString collumn_4;
    QString collumn_5;
    QString collumn_6;
    QString collumn_7;
    QString collumn_8;
    QString collumn_9;
    QString collumn_10;
    QString num;

    htm.append("<tr><td>"+column_name[i]+"</td><td>"+num.setNum(i)+"</td><td>"+collumn_1.setNum(column_1[i])+"</td><td>"+collumn_2.setNum(column_2[i])+"</td><td>"+collumn_3.setNum(column_3[i])+"</td><td>"+collumn_4.setNum(column_4[i])+"</td><td>"+collumn_5.setNum(column_5[i])+"</td><td>"+collumn_6.setNum(column_6[i])+"</td><td>"+collumn_7.setNum(column_7[i])+"</td><td>"+collumn_8.setNum(column_8[i])+"</td><td>"+collumn_9.setNum(column_9[i])+"</td><td>"+collumn_10.setNum(column_10[i])+"</td></tr>");
}
htm.append("</table>");
ui->webView->setHtml(htm);
}
void Dialog_reports::make_report_24_forma_10_all_years()
{
    qDebug()<<"Start Generation forma 10";
    QMap<int, int> column_1;
    QMap<int, int> column_2;
    QMap<int, int> column_3;
    QMap<int, int> column_4;
    QMap<int, int> column_5;
    QMap<int, int> column_6;
    QMap<int, int> column_7;
    QMap<int, int> column_8;
    QMap<int, int> column_9;
    QMap<int, int> column_10;
    QMap<int, QString> column_name;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*)\
                   FROM test.diagnos_patient,\
                   test.group_diagnos, test.patient, test.medcard, test.age_patients\
                   WHERE \
                   medcard.id = diagnos_patient.medcard_id AND\
                patient.id = medcard.patient_id AND\
                age_patients.id = medcard.id\
                AND patient.status in (99,100,101)\
                AND\
                patient.delete_row = 'f'\
                AND\
                medcard.sex = 'f' AND \
                (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false \
                group by group_diagnos.id order by group_diagnos.id ");//women (Забыл как по Русски пишется=)
                if(query.lastError().isValid())
        {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
                while(query.next())
        {
                column_2[query.value(0).toInt()] = query.value(2).toInt();
    }
}

if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id \
            AND patient.status in (99,100,101) \
            AND\
            age_patients.date_part >= 0 AND age_patients.date_part <= 14 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_3[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id \
            AND patient.status in (99,100,101)\
            AND\
            age_patients.date_part >= 15 AND age_patients.date_part <= 17 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_4[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id  ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id \
            AND patient.status in (99,100,101) \
            AND\
            age_patients.date_part >= 18 AND age_patients.date_part <= 19 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_5[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id  ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id \
            AND patient.status in (99,100,101)\
            AND\
            age_patients.date_part >= 20 AND age_patients.date_part <= 39 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_6[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id , group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id \
            AND patient.status in (99,100,101) \
            AND\
            age_patients.date_part >= 40 AND age_patients.date_part <= 59 AND\
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_7[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id ,group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            patient.status in (99,100,101) AND\
            patient.delete_row = 'f'\
            AND\
            age_patients.date_part >= 60 AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_8[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id , group_diagnos.name,count(*) \
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            dynamic_view.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            patient.delete_row = 'f'	\
            AND\
            dynamic_view.group_disp_view in('1','2','3','4','5','6','7') AND dynamic_view.status = 'f' AND dynamic_view.delete_row = 'f'\
            AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_9[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT group_diagnos.id, group_diagnos.name, count(*)\
               FROM test.diagnos_patient,\
               test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view\
               WHERE \
               medcard.id = diagnos_patient.medcard_id AND\
            patient.id = medcard.patient_id AND\
            age_patients.id = medcard.id AND\
            dynamic_view.medcard_id = medcard.id\
            AND patient.status in (99,100,101) AND\
            patient.delete_row = 'f'	\
            AND\
            dynamic_view.group_disp_view in('10') AND dynamic_view.status = 'f' AND dynamic_view.delete_row = 'f'\
            AND \
            (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
            group by group_diagnos.id order by group_diagnos.id ");
            if(query.lastError().isValid())
    {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
}
            while(query.next())
    {
            column_10[query.value(0).toInt()] = query.value(2).toInt();
}
}
if(db.open())
{
    query.exec("SELECT id, name FROM test.group_diagnos;");
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    while(query.next())
    {
        column_name[query.value(0).toInt()] = query.value(1).toString();
    }
}

for(int i = 0; i<24;i++)
{
    column_1[i] = column_3[i]+column_4[i]+column_5[i]+column_6[i]+column_7[i]+column_8[i];
}

//    if(db.open())
//    {
//        query.exec("SELECT group_diagnos.id, group_diagnos.name, count(*)\
//                   FROM test.diagnos_patient,\
//                    test.group_diagnos, test.patient, test.medcard, test.age_patients , test.dynamic_view\
//                  WHERE \
//                    medcard.id = diagnos_patient.medcard_id AND\
//                    patient.id = medcard.patient_id AND\
//                    age_patients.id = medcard.id AND\
//                    dynamic_view.medcard_id = medcard.id\
//                    and\
//                  (diagnos_patient.diagnos_id = ANY (group_diagnos.array_diagnos_id)) AND diagnos_patient.id_parent is null AND  diagnos_patient.delete_row = false\
//                  group by group_diagnos.id order by group_diagnos.id ");
//                while(query.next())
//                {
//                    column_1[query.value(0).toInt()] = query.value(2).toInt();
//                }
//    }



qDebug()<<column_name;
htm.append("<table border='1'>");
htm.append("<tr><td>Код по МКБ-X(класс V,адаптированный для использованияв РФ</td><td>Номер строки</td><td>Всего</td><td>из них женщин</td><td>0-14 лет</td><td>15-17 лет</td><td>18-19 лет</td><td>20-39лет</td><td>40-59 лет</td><td>60 лет и выше</td><td>Диспансерные пациенты</td><td>консультативные пациенты</td>");
for(int i=1; i<25; i++)
{
    QString collumn_1;
    QString collumn_2;
    QString collumn_3;
    QString collumn_4;
    QString collumn_5;
    QString collumn_6;
    QString collumn_7;
    QString collumn_8;
    QString collumn_9;
    QString collumn_10;
    QString num;

    htm.append("<tr><td>"+column_name[i]+"</td><td>"+num.setNum(i)+"</td><td>"+collumn_1.setNum(column_1[i])+"</td><td>"+collumn_2.setNum(column_2[i])+"</td><td>"+collumn_3.setNum(column_3[i])+"</td><td>"+collumn_4.setNum(column_4[i])+"</td><td>"+collumn_5.setNum(column_5[i])+"</td><td>"+collumn_6.setNum(column_6[i])+"</td><td>"+collumn_7.setNum(column_7[i])+"</td><td>"+collumn_8.setNum(column_8[i])+"</td><td>"+collumn_9.setNum(column_9[i])+"</td><td>"+collumn_10.setNum(column_10[i])+"</td></tr>");
}
htm.append("</table>");
ui->webView->setHtml(htm);
}
void Dialog_reports::dynamic_invalid()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString html_table;
    QString year_period;
    QList <QListWidgetItem*> list_w = ui->listWidget_year->selectedItems();
    QListIterator<QListWidgetItem*> i(list_w);
    while(i.hasNext())
    {
       year_period =  i.next()->text();
    }

    QList <int> itog;
    itog.append(0);
    itog.append(0);
    itog.append(0);
    itog.append(0);
    itog.append(0);
    html_table.append("<table border = '1'>");
    html_table.append("<tr><td rowspan='2'>группа инвалидности до<div>переосвидетельствования</td><td colspan='2'>всего</td><td colspan='3'>группа инвалидности после<div>переосвидетельствования</td><td rowspan='2'>признаны трудоспособными</td></tr><tr>");
    html_table.append("<td>до<div>переосвидетельствования</td><td>после<div>переосвидетельствования</td><td>i</td><td>ii</td><td>iii</td></tr>");
    html_table.append("<tr><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td><td>7</td></tr>");
    QStringList group_table;
    group_table.append("I");
    group_table.append("II");
    group_table.append("III");
    group_table.append("всего:");
    group_table.append("%");
    group_table.append("+- % по сравнению <br>с предыдущим годом");
    //QMap one_period;
    for(int i = 0; i<=2;i++)
    {
        html_table.append("<tr>");
        html_table.append("<td>").append(group_table[i]).append("</td>");
        if(db.open())
        {
            QString iosif;
            iosif.setNum(i);
            query.exec("SELECT count(*) FROM test.invalid_filter WHERE invalid_filter.srok_ocherednogo_pereosvidet >= '"+year_period+"-01-01'::date AND invalid_filter.srok_ocherednogo_pereosvidet <= '"+year_period+"-12-31'::date AND group_inv = '"+iosif+"'");
            query.next();
            html_table.append("<td>").append(query.value(0).toString()).append("</td>");
            itog[3] = itog[3]+query.value(0).toInt();
        }
        if(db.open())
        {
            QString iosif;
            iosif.setNum(i);
            query.exec("SELECT count(*) FROM test.invalid_filter WHERE medcard_id in (SELECT medcard_id FROM test.invalid_filter WHERE invalid_filter.srok_ocherednogo_pereosvidet >= '"+year_period+"-01-01'::date AND invalid_filter.srok_ocherednogo_pereosvidet <= '"+year_period+"-12-31'::date AND group_inv = '"+iosif+"') AND date_peresmotra >= '01.01."+year_period+"' AND date_peresmotra <= '31.12."+year_period+"'");
            query.next();
            html_table.append("<td>").append(query.value(0).toString()).append("</td>");
            itog[4] = itog[4]+query.value(0).toInt();
        }
        for(int j = 0; j<=2;j++)
        {

            if(db.open())
            {
                QString joker;
                QString iosif;
                joker.setNum(j);
                iosif.setNum(i);
                query.exec("SELECT count(*)\
                           FROM test.invalid_filter WHERE medcard_id in (SELECT medcard_id\
                                                                         FROM test.invalid_filter WHERE invalid_filter.srok_ocherednogo_pereosvidet >= '"+year_period+"-01-01'::date AND invalid_filter.srok_ocherednogo_pereosvidet <= '"+year_period+"-12-31'::date AND group_inv = '"+iosif+"')\
                           AND date_peresmotra >= '01.01."+year_period+"' AND date_peresmotra <= '31.12."+year_period+"' AND group_inv = '"+joker+"'");
                        query.next();
                        html_table.append("<td>").append(query.value(0).toString()).append("</td>");
                itog[j] = itog[j]+query.value(0).toInt();

            }
        }
            html_table.append("<td>").append("</td>");
            html_table.append("</tr>");
       }
        QString one;
        QString two;
        QString three;
        QString four;
        QString five;
        html_table.append("<tr><td>всего:</td><td>"+four.setNum(itog[3])+"</td><td>"+five.setNum(itog[4])+"</td><td>"+one.setNum(itog[0])+"</td><td>"+two.setNum(itog[1])+"</td><td>"+three.setNum(itog[2])+"</td><td></td></tr>"
                                                                                                                                                              "<tr><td>%</td><td></td><td></td><td></td><td></td><td></td><td></td></tr>"
                                                                                                                                                              "<tr><td>+- % по сравнению<div>с предыдущим годом</td><td></td><td></td><td></td><td></td><td></td><td></td></tr>");
        html_table.append("</table>");
        qDebug()<<html_table;
        ui->webView->setHtml(html_table);

}
void Dialog_reports::clicked_list()
{

}
void Dialog_reports::start_generate()
{
    Objects_app obj;
    QSettings *settings = new QSettings(obj.path_settings,QSettings::IniFormat);

    QString path_blanks;
    QString text_html;
    QByteArray text;
    QFile file;
    path_blanks.append(settings->value("path_blanks").toString()).append("/");
    switch (global_num) {
    case 1:
        path_blanks.append("medcard.html");
        file.setFileName(path_blanks);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        text_html = text.data();
        ui->webView->setHtml(text_html);
        break;
    case 2:
        path_blanks.append("medcard.html");
        file.setFileName(path_blanks);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        text = file.readAll();
        text_html = text.data();
        ui->webView->setHtml(text_html);
    case 9:
        dynamic_invalid();
        break;
    case 19:
        make_report_19_forma_10();
        break;
    case 24:
        make_report_24_forma_10_all_years();
        break;

    }
}

