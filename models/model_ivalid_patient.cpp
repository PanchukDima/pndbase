#include "model_ivalid_patient.h"

model_ivalid_patient::model_ivalid_patient(QWidget *parent)
{

}
QVariant model_ivalid_patient::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 10)
        {
            switch (value.toInt()) {
            case 0:
                //Д
                return "Д";
                break;
            case 1:
                //Д-1
                return "Д-1";
                break;
            case 2:
                //Д-2
                return "Д-2";
                break;
        }
        }
        if(index.column() == 2)
        {
            if(value.toBool())
            {
               return "Закрыт";
            }
            else
            {
                return "Открыт";
            }
        }
        if(index.column() == 8)
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
        if(index.column() == 12)
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
