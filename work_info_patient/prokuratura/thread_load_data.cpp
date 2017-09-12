#include "thread_load_data.h"

thread_load_data::thread_load_data(QObject *parent) : QObject(parent)
{

}
void thread_load_data::start()
{
    QSqlDatabase db = QSqlDatabase::database();
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
        Sleep(300);
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

            Sleep(300);
            if(db.open())
            {
                query.exec("SELECT  concat(patient.fname,' ',patient.name,' ',patient.mname) FROM test.medcard, test.patient WHERE medcard.patient_id = patient.id AND medcard.id="+medcard_id[i]);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                }
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";

                }
            }
            Sleep(300);
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
            Sleep(300);
            if(db.open())
            {
                query.exec("SELECT diagnos.code FROM (SELECT fixing_diagnos_date ,diagnos_id, medcard_id, rank() OVER (PARTITION by medcard_id ORDER by fixing_diagnos_date DESC) FROM  test.diagnos_patient WHERE delete_row = false AND id_parent is null) AS date_diagnos, test.diagnos WHERE rank = 1 AND diagnos.id = diagnos_id AND medcard_id ="+medcard_id[i]);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                }
                while (query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }

            }
            Sleep(300);
            if(db.open())
            {
                query.exec("SELECT min FROM test.max_and_min_dynamic WHERE medcard_id = "+medcard_id[i]);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                }
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
