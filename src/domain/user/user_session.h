#ifndef USER_SESSION_H
#define USER_SESSION_H

#include <QList>
#include <QString>
#include <QStringList>

namespace domain {

struct UserSession
{
    QString staffId;
    QString staffName;
    int staffPosition = 0;
    QList<bool> rights;
    QStringList servicingAreas;
    QString areaSqlList;
    QString departmentId;

    bool isAuthenticated() const noexcept
    {
        return !staffId.isEmpty();
    }

    bool hasRight(int index) const noexcept
    {
        return index >= 0 && index < rights.size() && rights.at(index);
    }

    void clear()
    {
        staffId.clear();
        staffName.clear();
        staffPosition = 0;
        rights.clear();
        servicingAreas.clear();
        areaSqlList.clear();
        departmentId.clear();
    }
};

} // namespace domain

#endif // USER_SESSION_H
