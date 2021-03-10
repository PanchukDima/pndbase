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
    qDebug()<<num+"report gen";


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
void Dialog_reports::make_report_vyt()
{

    QString html_table;
    html_table.append("<table border='1'>");
    html_table.append(make_report_vyt_sql(0,"1","true"));
    html_table.append(make_report_vyt_sql(0,"2","true"));
    html_table.append(make_report_vyt_sql(0,"3","true"));
    html_table.append(make_report_vyt_sql(0,"4","true"));
    html_table.append(make_report_vyt_sql(1,"1,2,3,4","true"));
    html_table.append(make_report_vyt_sql(2,"","true"));
    html_table.append(make_report_vyt_sql(0,"1","false"));
    html_table.append(make_report_vyt_sql(0,"2","false"));
    html_table.append(make_report_vyt_sql(0,"3","false"));
    html_table.append(make_report_vyt_sql(0,"4","false"));
    html_table.append(make_report_vyt_sql(1,"1,2,3,4","false"));
    html_table.append(make_report_vyt_sql(2,"","false"));
    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}
QString Dialog_reports::make_report_vyt_sql(int param, QString id_group,QString sex)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    qDebug()<<"До";
    QString a1;
    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
    int a8;
    int a9;
    int a10;
    int a11;
    int a12;
    int a13;
    int a14;
    if(id_group == "1" && param==0)
    {
        a1="F20.0 - F25.9";
    }
    else if(id_group =="2" && param==0)
    {
        a1="F06.0 - F07.9";
    }
    else if(id_group == "3" && param==0)
    {
        a1="F31.0-F39";
    }
    else if(id_group=="4" && param==0)
    {
        a1="F40.0-F48";
    }
    else if(param==1)
    {
        a1="Прочие заболевания";
    }
    else if( param==2)
    {
        a1="F00-F99";
    }
    a2=0;
    a3=0;
    a4=0;
    a5=0;
    a6=0;
    a7=0;
    a8=0;
    a9=0;
    a10=0;
    a11=0;
    a12=0;
    a13=0;
    a14=0;
    qDebug()<<"После";
    if(db.open())
    {
        switch (param) {
        case 0:
            query.exec("SELECT notwork_list.count_day, age_patients.date_part AS age, medcard.sex FROM   test.notwork_list,   test.age_patients, test.medcard\
                       WHERE   age_patients.id = notwork_list.medcard_id AND medcard.id = notwork_list.medcard_id AND delete_row = false AND state = true AND medcard.sex = "+sex+"  AND diagnos_id in (\
                        SELECT unnest(analize_vyt_group_diagnos.group_diagnos) FROM reports_library.analize_vyt_group_diagnos WHERE id = "+id_group+");");
                    if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }


            break;
        case 1:
            query.exec("SELECT notwork_list.count_day, age_patients.date_part AS age, medcard.sex FROM   test.notwork_list,   test.age_patients, test.medcard\
                       WHERE   age_patients.id = notwork_list.medcard_id AND medcard.id = notwork_list.medcard_id AND delete_row = false AND state = true AND medcard.sex = "+sex+"  AND diagnos_id not in (\
                        SELECT unnest(analize_vyt_group_diagnos.group_diagnos) FROM reports_library.analize_vyt_group_diagnos WHERE id in("+id_group+"));");
                    if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }
            break;
        case 2:
            query.exec("SELECT notwork_list.count_day, age_patients.date_part AS age, medcard.sex FROM   test.notwork_list,   test.age_patients, test.medcard\
                       WHERE   age_patients.id = notwork_list.medcard_id AND medcard.id = notwork_list.medcard_id AND delete_row = false AND state = true  AND medcard.sex = "+sex+"");
                    if(query.lastError().isValid())
            {
                    qDebug()<<query.lastError();
        }
                    break;
        }
        while(query.next())
        {
            qDebug()<<query.value(0).toString()<<query.value(1).toString()<<query.value(2).toString();
            a2=a2+query.value(0).toInt();
            if(query.value(1).toInt()>=15 && query.value(1).toInt()<=15)
            {
                ++a4;
            }
            else if(query.value(1).toInt()>=20 && query.value(1).toInt()<=24)
            {
                ++a5;
            }
            else if(query.value(1).toInt()>=25 && query.value(1).toInt()<=29)
            {
                ++a6;
            }
            else if(query.value(1).toInt()>=30 && query.value(1).toInt()<=34)
            {
                ++a7;
            }
            else if(query.value(1).toInt()>=35 && query.value(1).toInt()<=39)
            {
                ++a8;
            }
            else if(query.value(1).toInt()>=40 && query.value(1).toInt()<=44)
            {
                ++a9;
            }
            else if(query.value(1).toInt()>=45 && query.value(1).toInt()<=49)
            {
                ++a10;
            }
            else if(query.value(1).toInt()>=50 && query.value(1).toInt()<=54)
            {
                ++a11;
            }
            else if(query.value(1).toInt()>=55 && query.value(1).toInt()<=59)
            {
                ++a12;
            }
            else if(query.value(1).toInt()>=60 )
            {
                ++a13;
            }

        }
        qDebug()<<a3;
        a3 = a4+a5+a6+a7+a8+a9+a10+a11+a12+a13;
        if(a3==0)
        {
            a14 =0;
        }
        else
        {
            a14 = a2/a3;
        }
        return string_table_report(0).arg(a1).arg(a2).arg(a3).arg(a4).arg(a5).arg(a6).arg(a7).arg(a8).arg(a9).arg(a10).arg(a11).arg(a12).arg(a13).arg(a14);
    }
}

void Dialog_reports::make_report_1()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString html_table;
    html_table.append("<table border = '1'>");
    html_table.append("<tr><td>№ карты</td><td>ФИО</td><td>Д.Р.</td><td>Адрес</td><td>Госпитализация</td></tr>");
    if(db.open())
    {
        query.exec("SELECT \
                   hospitalization.medcard_id, \
                   concat( all_view_patient.fname, ' ' ,\
                   all_view_patient.name, ' ' ,\
                   all_view_patient.mname), \
                   all_view_patient.birthday, \
                   concat(street.name, \
                                    CASE\
                                      WHEN address_patient.building ='' THEN ''\
                                      ELSE ' корп.'\
                                    END , \
                                    address_patient.building, ' д.',\
                                    address_patient.home, ' кв.',\
                                    address_patient.flat) AS Street,\
                   hospitalization.date_up\
                 FROM \
                   test.all_view_patient, \
                   test.hospitalization, \
                   test.address_patient, \
                   test.street\
                 WHERE \
                   hospitalization.medcard_id = all_view_patient.id AND\
                   address_patient.medcard_id = all_view_patient.id AND\
                   street.id = address_patient.street_id;");
                while(query.next())
        {
                html_table.append("<tr><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toDate().toString("dd.MM.yyyy")+"</td><td>"+query.value(3).toString()+"</td><td>"+query.value(4).toDate().toString("dd.MM.yyyy") +"</td></tr>");
        }
    }
    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}
void Dialog_reports::make_report_2()
{
    QString html_table;
    html_table.append("<table border='1'>");
    for(int i=1;i<=27;i++)
    {
    html_table.append(make_report_2_sql(QString::number(i)));
    }
    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}

QString Dialog_reports::make_report_2_sql(QString row)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString out = "<tr><td>%1</td><td>%2</td><td>%3</td></tr>";
    QList<int> arr;
    for(int i=0;i<=3;i++)
    {
    arr.append(0);
    }
    if(db.open())
    {
        query.exec("SELECT hospitalization.medcard_id, who_wrote, doctor, date_up, date_down, \
                   hospitalization.delete_row, diagnos_patient.diagnos_id, hospitalization.first\
              FROM test.hospitalization , test.diagnos_patient WHERE diagnos_patient.id = hospitalization.diagnose_id AND hospitalization.delete_row = false AND diagnos_patient.diagnos_id in(SELECT unnest(analize_vyt_group_diagnos.group_diagnos) FROM reports_library.analize_vyt_group_diagnos WHERE id in("+row+"))");
                while(query.next())
        {
            if(query.value(1).toInt()==1)
            {

                ++arr[1];
            }
            else if(query.value(1).toInt()==0)
            {
                ++arr[2];
            }
            else if(query.value(1).toInt()==2)
            {
                ++arr[3];
            }
        }

        return out.arg(arr[1]).arg(arr[2]).arg(arr[3]);
    }
    
}
void Dialog_reports::make_report_7()
{
    QString html_table;
    html_table.append("<table border='1'>");
    for(int i=1;i<=3;i++)
    {
    html_table.append(make_report_12_sql(QString::number(i)));
    }
    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}
QString Dialog_reports::make_report_7_sql(QString key)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString out = "<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td><td>%10</td><td>%11</td><td>%12</td><td>%13</td><td>%14</td><td>%15</td><td>%16</td><td>%17</td><td>%18</td><td>%19</td><td>%20</td><td>%21</td><td>%22</td><td>%23</td><td>%24</td><td>%25</td><td>%26</td><td>%27</td><td>%28</td><td>%29</td><td>%30</td><td>%31</td><td>%32</td><td>%33</td><td>%34</td><td>%35</td><td>%36</td><td>%37</td><td>%38</td><td>%39</td><td>%40</td><td>%41</td><td>%42</td><td>%43</td><td>%44</td><td>%45</td><td>%46</td></tr>";
    QList<int> arr;
    for(int i=0;i<=46;i++)
    {
    arr.append(0);
    }
    if(db.open())
    {
        query.exec("SELECT  medcard.area_id FROM test.hospitalization, test.medcard WHERE medcard.id = hospitalization.medcard_id AND hospitalization.who_wrote = '"+key+"'");
                while(query.next())
        {
            if(query.value(1).toInt()==99 ||query.value(1).toInt()==100 ||query.value(1).toInt()==101)
            {

                ++arr[(query.value(0).toInt()*2)-1];
            }
            else
            {
                ++arr[(query.value(0).toInt()*2)];
            }
        }

        return out.arg(arr[1]).arg(arr[2]).arg(arr[3]).arg(arr[4]).arg(arr[5]).arg(arr[6]).arg(arr[7]).arg(arr[8]).arg(arr[9]).arg(arr[10]).arg(arr[11]).arg(arr[12]).arg(arr[13]).arg(arr[14]).arg(arr[15]).arg(arr[16]).arg(arr[17]).arg(arr[18]).arg(arr[19]).arg(arr[20]).arg(arr[21]).arg(arr[22]).arg(arr[23]).arg(arr[24]).arg(arr[25]).arg(arr[26]).arg(arr[27]).arg(arr[28]).arg(arr[29]).arg(arr[30]).arg(arr[31]).arg(arr[32]).arg(arr[33]).arg(arr[34]).arg(arr[35]).arg(arr[36]).arg(arr[37]).arg(arr[38]).arg(arr[39]).arg(arr[40]).arg(arr[41]).arg(arr[42]).arg(arr[43]).arg(arr[44]).arg(arr[45]).arg(arr[46]);
    }
}
void Dialog_reports::make_report_12()
{
    QString html_table;
    html_table.append("<table border='1'>");
    for(int i=0;i<=21;i++)
    {
    html_table.append(make_report_12_sql(QString::number(i)));
    }
    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}
QString Dialog_reports::make_report_12_sql(QString key)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString out = "<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td><td>%10</td><td>%11</td><td>%12</td><td>%13</td><td>%14</td><td>%15</td><td>%16</td><td>%17</td><td>%18</td><td>%19</td><td>%20</td><td>%21</td><td>%22</td><td>%23</td><td>%24</td><td>%25</td><td>%26</td><td>%27</td><td>%28</td><td>%29</td><td>%30</td><td>%31</td><td>%32</td><td>%33</td><td>%34</td><td>%35</td><td>%36</td><td>%37</td><td>%38</td><td>%39</td><td>%40</td><td>%41</td><td>%42</td><td>%43</td><td>%44</td><td>%45</td><td>%46</td></tr>";
    QList<int> arr;
    for(int i=0;i<=46;i++)
    {
    arr.append(0);
    }


    if(db.open())
    {
        query.exec("SELECT all_view_patient.area_id, all_view_patient.status\
                   FROM test.diagnos_patient, test.all_view_patient WHERE all_view_patient.id = diagnos_patient.medcard_id AND diagnos_patient.delete_row = false AND diagnos_patient.diagnos_id in \
                   (	SELECT unnest(group_diagnos.array_diagnos_id) FROM test.group_diagnos WHERE id ="+key+" );");
                while(query.next())
        {
            if(query.value(1).toInt()==99 ||query.value(1).toInt()==100 ||query.value(1).toInt()==101)
            {

                ++arr[(query.value(0).toInt()*2)-1];
            }
            else
            {
                ++arr[(query.value(0).toInt()*2)];
            }
        }

        return out.arg(arr[1]).arg(arr[2]).arg(arr[3]).arg(arr[4]).arg(arr[5]).arg(arr[6]).arg(arr[7]).arg(arr[8]).arg(arr[9]).arg(arr[10]).arg(arr[11]).arg(arr[12]).arg(arr[13]).arg(arr[14]).arg(arr[15]).arg(arr[16]).arg(arr[17]).arg(arr[18]).arg(arr[19]).arg(arr[20]).arg(arr[21]).arg(arr[22]).arg(arr[23]).arg(arr[24]).arg(arr[25]).arg(arr[26]).arg(arr[27]).arg(arr[28]).arg(arr[29]).arg(arr[30]).arg(arr[31]).arg(arr[32]).arg(arr[33]).arg(arr[34]).arg(arr[35]).arg(arr[36]).arg(arr[37]).arg(arr[38]).arg(arr[39]).arg(arr[40]).arg(arr[41]).arg(arr[42]).arg(arr[43]).arg(arr[44]).arg(arr[45]).arg(arr[46]);
    }
}
void Dialog_reports::make_report_17()
{
    QString html_table;
    html_table.append("<table border='1'>");

    html_table.append(make_report_17_sql("20","24"));
    html_table.append(make_report_17_sql("25","29"));
    html_table.append(make_report_17_sql("30","34"));
    html_table.append(make_report_17_sql("35","39"));
    html_table.append(make_report_17_sql("40","44"));
    html_table.append(make_report_17_sql("45","49"));
    html_table.append(make_report_17_sql("50","54"));
    html_table.append(make_report_17_sql("55","59"));
    html_table.append(make_report_17_sql("60","99999"));

    html_table.append("</table>");
    ui->webView->setHtml(html_table);
}
QString Dialog_reports::make_report_17_sql(QString start,QString end)
{

     QSqlDatabase db = QSqlDatabase::database();
     QSqlQuery query;
     QString out = "<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>";
     QList<int> arr;
     for(int i=0;i<=4;i++)
     {
     arr.append(0);
     }
     if(db.open())
     {
        query.exec("SELECT medcard_id, sex, date_part, count_day FROM test.notwork_list, test.medcard, test.age_patients WHERE medcard.id = notwork_list.medcard_id AND age_patients.id = medcard.id AND date_part >="+start+" AND date_part <= "+end+";");
        while(query.next())
        {
            if(query.value(1).toBool())
            {
                ++arr[1];
                arr[3]=arr[3]+query.value(3).toInt();
            }
            else
            {
                ++arr[2];
                arr[4]=arr[4]+query.value(3).toInt();
            }
        }
     return out.arg(arr[1]).arg(arr[2]).arg(arr[3]).arg(arr[4]);
     }


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
    case 0:
        make_report_vyt();
        break;
    case 1:
        make_report_1();
        break;
    case 2:
        make_report_2();
//        path_blanks.append("medcard.html");
//        file.setFileName(path_blanks);
//        file.open(QIODevice::ReadOnly | QIODevice::Text);
//        text = file.readAll();
//        text_html = text.data();
//        ui->webView->setHtml(text_html);
        break;
    case 7:
        make_report_7();
        break;
    case 9:
        dynamic_invalid();
        break;
    case 12:
        make_report_12();
        break;
    case 15:
        make_report_15_suicid();
        qDebug()<<"15 report";
        break;
    case 17:
        make_report_17();
        break;
    case 19:
        make_report_19_forma_10();
        break;
    case 24:
        make_report_24_forma_10_all_years();
        break;

    }
}
void Dialog_reports::make_report_15_suicid()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString html_table;
    html_table.append("<table border = '1'> <tr><td>№ Карты</td><td>ФИО</td><td >Дата рождения</td><td>Адрес</td><td>Дата суицида</td><td>Тип суицида</td></tr>");
    if(db.open())
    {
        query.exec("SELECT \
                   medcard.id, \
                   concat(patient.fname, ' ',\
                   patient.name, ' ',\
                   patient.mname) AS FIO,\
                   medcard.birthday, \
                   concat(street.name, \
                   CASE\
                     WHEN address_patient.building ='' THEN ''\
                     ELSE ' корп.'\
                   END , \
                   address_patient.building, ' д.',\
                   address_patient.home, ' кв.',\
                   address_patient.flat) AS Street,\
                   suicide.datestart, \
                   CASE\
                   WHEN suicide.tsuicide_id ='0' THEN 'Демонстративный'\
                   ELSE 'Истинный'\
                                      END  \
                   \
                 FROM \
                   test.suicide, \
                   test.medcard, \
                   test.patient, \
                   test.address_patient, \
                   test.street\
                 WHERE \
                   medcard.id = suicide.medcard_id AND\
                   patient.id = medcard.patient_id AND\
                   address_patient.medcard_id = medcard.id AND\
                   street.id = address_patient.street_id;");
                while(query.next())
        {
            QString birthday = query.value(2).toDate().toString("dd.MM.yyyy");
            QString date_suicide =query.value(4).toDate().toString("dd.MM.yyyy");
            html_table.append("<tr> <td>"+query.value(0).toString()+"</td> <td>"+query.value(1).toString()+"</td> <td>"+birthday+"</td> <td>"+query.value(3).toString()+"</td><td>"+date_suicide+"</td><td>"+query.value(5).toString()+"</td></tr>");
        }
        html_table.append("</table>");
        ui->webView->setHtml(html_table);
    }
}
QString Dialog_reports::string_table_report(int key)
{
    switch (key) {
    case 0:
        return "<tr> <td>%1</td> <td>%2</td> <td>%3</td> <td>%4</td> <td>%5</td> <td>%6</td> <td>%7</td> <td>%8</td> <td>%9</td> <td>%10</td> <td>%11</td> <td>%12</td> <td>%13</td> <td>%14</td></tr>";
        break;
    default:
        break;
    }
}
