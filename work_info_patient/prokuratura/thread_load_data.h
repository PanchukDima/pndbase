#ifndef THREAD_LOAD_DATA_H
#define THREAD_LOAD_DATA_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>
#include <windows.h>
#include <QThread>
#include <QSettings>
#include "Objects/User/objects_app.h"

class thread_load_data : public QObject
{
    Q_OBJECT
public:
    explicit thread_load_data(QObject *parent = nullptr);
    QString file;

signals:
    void finished();
    void status(int);
    void progress(int);
    void progress_max(int);

public slots:
    void start();
};

#endif // THREAD_LOAD_DATA_H
