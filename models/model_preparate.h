#ifndef MODEL_PREPARATE_H
#define MODEL_PREPARATE_H

#include <QObject>
#include <QSqlTableModel>
#include <QDate>
#include <QColor>

class model_preparate : public QSqlTableModel
{
    Q_OBJECT
public:
    model_preparate(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


signals:

public slots:

};

#endif // MODEL_PREPARATE_H
