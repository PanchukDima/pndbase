#include "application_context.h"

ApplicationContext::ApplicationContext(QObject *parent)
{
    Q_UNUSED(parent)
}

ApplicationContext &ApplicationContext::instance() noexcept
{
    static ApplicationContext context;
    return context;
}

domain::UserSession &ApplicationContext::userSession() noexcept
{
    return userSession_;
}

const domain::UserSession &ApplicationContext::userSession() const noexcept
{
    return userSession_;
}

core::ApplicationPaths &ApplicationContext::paths() noexcept
{
    return paths_;
}

const core::ApplicationPaths &ApplicationContext::paths() const noexcept
{
    return paths_;
}

QString &ApplicationContext::staff_id = ApplicationContext::instance().userSession().staffId;
QString &ApplicationContext::staff_name = ApplicationContext::instance().userSession().staffName;
int &ApplicationContext::staff_position = ApplicationContext::instance().userSession().staffPosition;
QList<bool> &ApplicationContext::rights_user = ApplicationContext::instance().userSession().rights;
QStringList &ApplicationContext::servising_area = ApplicationContext::instance().userSession().servicingAreas;
QString &ApplicationContext::str_area_list = ApplicationContext::instance().userSession().areaSqlList;
QString &ApplicationContext::path_settings = ApplicationContext::instance().paths().settingsFile;
QString &ApplicationContext::department_id = ApplicationContext::instance().userSession().departmentId;
