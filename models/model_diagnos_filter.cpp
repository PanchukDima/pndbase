#include "model_diagnos_filter.h"

model_diagnos_filter::model_diagnos_filter(QWidget *parent)
{

}
QVariant model_diagnos_filter::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
        switch(role){
         case Qt::DisplayRole:
           if(index.column() == 7)
            {
                if(value.toBool())
                {
                    return "M";
                }
                else
                {
                    return "Ж";
                }
            }
            if(index.column() == 10)
            {
                if(value.toInt()==1)
                {
                    return "Выборгский";
                }
                if(value.toInt()==2)
                {
                    return "Калининский";
                }
            }
        }
        return value;
}
