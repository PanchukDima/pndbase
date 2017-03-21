#include "objects_app.h"
QString Objects_app::staff_id;
QString Objects_app::staff_name;
int Objects_app::staff_position;
QList <bool> Objects_app::rights_user;
QStringList Objects_app::servising_area;
QString Objects_app::str_area_list;
Objects_app::Objects_app(QObject *parent) : QObject(parent)
{

}
