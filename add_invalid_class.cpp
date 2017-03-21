#include "add_invalid_class.h"

add_invalid_class::add_invalid_class(QObject *parent) : QObject(parent)
{

}

void add_invalid_class::send_data(QString global_id, QString global_staff_id, QString group_invalid_value, QString from_other_value, QString work_yes_value, QString date_inst, QString date_peresmotr, QString inv_child, bool bs, QString first_value,QString place_work_id, QString step_lost, QString other_recomm,QString dubl)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString diagnos_id;
    if(db.open())
    {
        query.exec("SELECT diagnos_patient.id FROM test.diagnos_patient WHERE id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+global_id+" AND delete_row = 'false' AND id_parent is NULL);");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();

        }
        else
        {
            while(query.next())
            {
                diagnos_id = query.value(0).toString();
            }
        }
    }
    if(db.open())
    {
        if(bs)
        {
             query.exec("INSERT INTO test.invalid_patient(medcard_id, staff_add_id, group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs,inv_child, delete_row, diagnos_id, first, place_work_id, stepen_utrachenoy_trudosposobnosti, dubl, other_recomendation, staff_edit_id) VALUES ('"+global_id+"', '"+global_staff_id+"', '"+group_invalid_value+"', '"+work_yes_value+"', '"+from_other_value+"', '"+date_inst+"', NULL, true,'"+inv_child+"', false, "+diagnos_id+",'"+first_value+"','"+place_work_id+"','"+step_lost+"','"+dubl+"','"+other_recomm+"', 0);");
             if(query.lastError().isValid())
             {
                 qDebug()<<query.lastError();

             }
             else
             {
             query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена инвалидность в медкарту № "+global_id+"')");
             if(query.lastError().isValid())
             {
                 qDebug()<<query.lastError();

             }
             }
             }
        else
        {
            query.exec("INSERT INTO test.invalid_patient(medcard_id, staff_add_id, group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs,inv_child, delete_row, diagnos_id, first, place_work_id, stepen_utrachenoy_trudosposobnosti, dubl, other_recomendation, staff_edit_id) VALUES ('"+global_id+"', '"+global_staff_id+"', '"+group_invalid_value+"', '"+work_yes_value+"', '"+from_other_value+"', '"+date_inst+"', '"+date_peresmotr+"', false,'"+inv_child+"', false, "+diagnos_id+", '"+first_value+"','"+place_work_id+"','"+step_lost+"','"+dubl+"','"+other_recomm+"', 0);");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();

            }
            else
            {
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+global_staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Добавлена инвалидность в медкарту № "+global_id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();

            }
            }
            }

    }
    else
    {

    }
    emit send_ok();

}

