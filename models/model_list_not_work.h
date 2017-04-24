#ifndef MODEL_LIST_NOT_WORK_H
#define MODEL_LIST_NOT_WORK_H

#include <QObject>
#include <QSqlTableModel>

class model_list_not_work : public QSqlTableModel
{
    Q_OBJECT
public:
    model_list_not_work(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
signals:

public slots:
};

#endif // MODEL_LIST_NOT_WORK_H
