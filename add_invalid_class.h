#ifndef ADD_INVALID_CLASS_H
#define ADD_INVALID_CLASS_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

class add_invalid_class : public QObject
{
    Q_OBJECT
public:
    explicit add_invalid_class(QObject *parent = 0);
signals:
    void send_ok();
public slots:
    void send_data(QString , QString , QString , QString , QString , QString , QString , QString, bool, QString, QString, QString, QString, QString);
};

#endif // ADD_INVALID_CLASS_H
