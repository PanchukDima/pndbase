#ifndef MODEL_VISITING_CONTROL_ALL_H
#define MODEL_VISITING_CONTROL_ALL_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QDate>

class model_visiting_control_all : public QSqlTableModel
{
    Q_OBJECT
public:
    model_visiting_control_all(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_VISITING_CONTROL_ALL_H
