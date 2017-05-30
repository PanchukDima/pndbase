#ifndef MODEL_DATA_ANALIZES_H
#define MODEL_DATA_ANALIZES_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QDate>

class model_data_analizes : public QSqlTableModel
{
    Q_OBJECT
public:
    model_data_analizes(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_DATA_ANALIZES_H
