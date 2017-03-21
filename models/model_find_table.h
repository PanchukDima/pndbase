#ifndef MODEL_FIND_TABLE_H
#define MODEL_FIND_TABLE_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QProgressBar>

class model_find_table : public QSqlTableModel
{
    Q_OBJECT
public:
     model_find_table(QWidget *parent = 0);
     QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

};

#endif // MODEL_FIND_TABLE_H
