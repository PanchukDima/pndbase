#include "models/model_main_table.h"

model_main_table::model_main_table(QWidget *parent)
{

}
QVariant model_main_table::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
        case Qt::BackgroundRole:
            if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 0)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 1)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 3)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 4)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 5)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 9)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 10)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 11)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else if(index.sibling(index.row(),6).data( Qt::DisplayRole ).toInt() == 14)
            {
                return qVariantFromValue(QColor(Qt::lightGray));
            }
            else
            {
                return qVariantFromValue(QColor(Qt::white));
            }
        case Qt::DisplayRole:
            if(index.column() == 7)
            {
                return value.toString().append(" участок");
            }
    }
    return  QSqlTableModel::data(index, role);

}
