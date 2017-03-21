#ifndef MODEL_MAIN_TABLE_H
#define MODEL_MAIN_TABLE_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>

class model_main_table : public QSqlTableModel
{
    Q_OBJECT
public:
    model_main_table(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

};

#endif // MODEL_MAIN_TABLE_H
