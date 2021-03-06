#include "thread_load_data.h"

thread_load_data::thread_load_data(QObject *parent) : QObject(parent)
{

}
void thread_load_data::start()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "DBThread");
    QSettings *settings = new QSettings(obj.path_settings,QSettings::IniFormat);
    QString ipdatabase = settings->value("ipdatabase").toString();
    int portdatabase = settings->value("portdatabase").toInt();
    QString databasename = settings->value("databasename").toString();
    QString username = settings->value("username").toString();
    QString password = settings->value("password").toString();
    db.setHostName("192.168.0.150");
    db.setDatabaseName("newpsqldb");
    db.setPort(5432);
    db.setConnectOptions("application_name = BDPatient_Thread");
    db.setUserName("sysadmindb");
    db.setPassword("Wsx345876ijN");
    QSqlQuery query;
    QStringList medcard_id;
    QStringList destroy;
    emit progress(0);
    emit status(0);

    if(db.open())
    {
        query.exec("SELECT dynamic_view.medcard_id FROM test.dynamic_view WHERE delete_row = false AND status = false AND group_disp_view in ('0','1','2','3','4','5','6','7')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
        while(query.next())
        {
            medcard_id.append(query.value(0).toString());
        }
    }
    emit progress_max(medcard_id.count()-1);
    for(int i=0;medcard_id.count()>i;i++)
    {

        emit progress(i);
        if(db.open())
        {
            query.exec("SELECT patient.status FROM test.patient, test.medcard WHERE medcard.patient_id = patient.id AND delete_row = false AND medcard.id = "+medcard_id[i]);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }
            while(query.next())
            {
                if(query.value(0).toInt()<99)
                {
                    destroy.append(medcard_id[i]);
                }
            }
        }
    }

    emit progress(0);
    emit progress_max(destroy.count()-1);
    emit status(1);

    for(int i =0; destroy.count()>i;i++)
    {
        emit progress(i);

        for(int y=0;medcard_id.count()>y;y++)
        {
            if(medcard_id[y]==destroy[i])
            {
                medcard_id.removeAt(y);
            }
        }
    }

    emit progress(0);
    emit status(2);
    emit progress_max(medcard_id.count()-1);

    QFile f(file);
    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        for(int i =0;medcard_id.count()>i;i++)
        {
            if(db.open())
            {
                query.exec("SELECT  concat(patient.fname,' ',patient.name,' ',patient.mname) FROM test.medcard, test.patient WHERE medcard.patient_id = patient.id AND medcard.id="+medcard_id[i]);
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";

                }
            }
            if(db.open())
            {

                query.exec("SELECT medcard.birthday FROM test.medcard WHERE medcard.id = "+medcard_id[i]);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                }
                while(query.next())
                {
                    strList << "\""+query.value(0).toDate().toString("dd.MM.yyyy")+"\"";
                }
            }
            if(db.open())
            {
                query.exec("SELECT diagnos.code FROM (SELECT fixing_diagnos_date ,diagnos_id, medcard_id, rank() OVER (PARTITION by medcard_id ORDER by fixing_diagnos_date DESC) FROM  test.diagnos_patient WHERE delete_row = false AND id_parent is null) AS date_diagnos, test.diagnos WHERE rank = 1 AND diagnos.id = diagnos_id AND medcard_id ="+medcard_id[i]);
                while (query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }

            }
            if(db.open())
                        {
                            query.exec("SELECT concat(street.name, CASE WHEN address_patient.building ='' THEN '' ELSE ' ????????.' END , address_patient.building, ' ??.', address_patient.home, ' ????.', address_patient.flat) AS Street FROM test.address_patient , test.street WHERE street.id = address_patient.street_id AND medcard_id = "+medcard_id[i]);
                            while(query.next())
                            {
                                strList << "\""+query.value(0).toString()+"\"";
                            }
                        }
            if(db.open())
                        {
                            query.exec("SELECT CASE WHEN medcard.district_id ='1' THEN '????????????????????' ELSE '??????????????????????' END FROM test.medcard WHERE medcard.id = "+medcard_id[i]);
                            while(query.next())
                            {
                                strList << "\""+query.value(0).toString()+"\"";
                            }
                        }
            if(db.open())
            {
                query.exec("SELECT min FROM test.max_and_min_dynamic WHERE medcard_id = "+medcard_id[i]);
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }
            }
            ts << strList.join( ";" )+"\n";
            strList.clear();
            emit progress(i);
        }

        emit status(3);
        f.close();

    }
    emit finished();

}
