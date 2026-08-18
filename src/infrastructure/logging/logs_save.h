#ifndef LOGS_SAVE_H
#define LOGS_SAVE_H

#include <QObject>
#include <QFile>
#include <QDir>

class logs_save : public QObject
{
    Q_OBJECT
public:
    explicit logs_save(QObject *parent = 0);

signals:

public slots:
    bool error_log(QString);
};

#endif // LOGS_SAVE_H
