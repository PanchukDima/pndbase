#include "model_preparate.h"

model_preparate::model_preparate(QWidget *parent)
{

}
QVariant model_preparate::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
        case Qt::BackgroundRole:
            if(index.sibling(index.row(),8).data( Qt::DisplayRole ).toDate() <= QDate::currentDate().addMonths(-6))
            {
                return qVariantFromValue(QColor(Qt::red));
            }
    }
    return  QSqlTableModel::data(index, role);
}
