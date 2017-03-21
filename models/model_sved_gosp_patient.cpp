#include "model_sved_gosp_patient.h"

model_sved_gosp_patient::model_sved_gosp_patient(QWidget *parent)
{

}
QVariant model_sved_gosp_patient::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 2)
        {
            switch (value.toInt()) {
            case 0:
                //Д
                return "СП";
                break;
            case 1:
                //Д-1
                return "ПНД";
                break;
            case 2:
                //Д-2
                return "Другое";
                break;
            }

        }


    }
    return value;

}
