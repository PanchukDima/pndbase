#ifndef MODEL_DIAGNOS_FILTER_H
#define MODEL_DIAGNOS_FILTER_H

#include <QObject>
#include <QSqlTableModel>

class model_diagnos_filter : public QSqlTableModel
{
    Q_OBJECT
public:
    model_diagnos_filter(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_DIAGNOS_FILTER_H
