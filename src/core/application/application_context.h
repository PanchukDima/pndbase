#ifndef APPLICATION_CONTEXT_H
#define APPLICATION_CONTEXT_H

#include "application_paths.h"
#include "domain/user/user_session.h"

class QObject;

class ApplicationContext
{
public:
    explicit ApplicationContext(QObject *parent = nullptr);

    static ApplicationContext &instance() noexcept;

    domain::UserSession &userSession() noexcept;
    const domain::UserSession &userSession() const noexcept;

    core::ApplicationPaths &paths() noexcept;
    const core::ApplicationPaths &paths() const noexcept;

    // Transitional aliases preserve the legacy call sites while state now lives
    // in explicit objects. New code should use instance().userSession()/paths().
    static QString &staff_id;
    static QString &staff_name;
    static int &staff_position;
    static QList<bool> &rights_user;
    static QStringList &servising_area;
    static QString &str_area_list;
    static QString &path_settings;
    static QString &department_id;

private:
    domain::UserSession userSession_;
    core::ApplicationPaths paths_;
};

// Compatibility name retained while legacy screens are migrated incrementally.
using Objects_app = ApplicationContext;

#endif // APPLICATION_CONTEXT_H
