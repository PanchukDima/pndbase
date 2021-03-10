#include "model_data_analizes.h"

model_data_analizes::model_data_analizes(QWidget *parent)
{

}
QVariant model_data_analizes::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::BackgroundRole:
        if(index.sibling(index.row(),7).data( Qt::DisplayRole ).toString() == "RW" && index.sibling(index.row(),6).data( Qt::DisplayRole ).toDate() <= QDate::currentDate().addYears(-1))
        {
            return qVariantFromValue(QColor(Qt::red));
        }
        if(index.sibling(index.row(),7).data( Qt::DisplayRole ).toString() == "Туберкулез" && index.sibling(index.row(),6).data( Qt::DisplayRole ).toDate() <= QDate::currentDate().addMonths(-6))
        {
            return qVariantFromValue(QColor(Qt::red));
        }
    case Qt::DisplayRole:
        if(index.column() == 7)
        {
            switch (value.toInt()) {
            case 1:
                return "Туберкулез";
                break;
            case 2:
                return "RW";
                break;
            }
        }

    }
    return  QSqlTableModel::data(index, role);
}
