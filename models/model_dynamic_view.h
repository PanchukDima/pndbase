#ifndef MODEL_DYNAMIC_VIEW_H
#define MODEL_DYNAMIC_VIEW_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>

class model_dynamic_view : public QSqlTableModel
{
    Q_OBJECT
public:
    model_dynamic_view(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


signals:

public slots:
};

#endif // MODEL_DYNAMIC_VIEW_H
