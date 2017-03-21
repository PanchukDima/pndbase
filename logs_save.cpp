#include "logs_save.h"

logs_save::logs_save(QObject *parent) : QObject(parent)
{

}
bool logs_save::error_log(QString string)
{
    QFile file("logs/log_error.txt");
    QDir dir("logs");

    if(!dir.exists())
    {
        QDir().mkdir("logs");
    }
    if(file.open(QIODevice::Append))
    {
        QString line;
        line.append(string).append("\n");
        file.write(line.toUtf8());
        return true;
        file.close();
    }
    else
    {
        return false;
    }

}
