#ifndef OBJECTS_APP_H
#define OBJECTS_APP_H

#include <QObject>

class Objects_app : public QObject
{
    Q_OBJECT
public:
    explicit Objects_app(QObject *parent = 0);
    static QString staff_id;
    static QString staff_name;
    static int staff_position;
    static QList <bool> rights_user;
    static QStringList servising_area;
    static QString str_area_list;
    static QString path_settings;



signals:

public slots:
};


#endif // OBJECTS_APP_H
