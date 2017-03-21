#include "new_changes_thread.h"


new_changes_thread::new_changes_thread(QObject *parent) : QObject(parent)
{


}

new_changes_thread::~new_changes_thread()
{

}
void new_changes_thread::check_new_changes_blanks()
{
    while(true)
    {



#ifdef _WIN32
  Sleep(5000);
#endif
        get_session_id();






    }
}
void new_changes_thread::get_session_id()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    if(db.open())
    {


        query.exec("SELECT  text_task, status, session_id FROM test.tasks WHERE session_id="+global_session_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();

        }
        while(query.next())
        {
            QString text = query.value(0).toString();
            switch (query.value(1).toInt()) {
            case 1:
                exit(0);
                break;
            case 2:

                break;
            }
        }
    }
}

