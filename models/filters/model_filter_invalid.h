#ifndef MODEL_FILTER_INVALID_H
#define MODEL_FILTER_INVALID_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>

class model_filter_invalid : public QSqlTableModel
{
    Q_OBJECT
public:
    model_filter_invalid(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_FILTER_INVALID_H
