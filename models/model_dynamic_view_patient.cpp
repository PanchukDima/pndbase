#include "model_dynamic_view_patient.h"

model_dynamic_view_patient::model_dynamic_view_patient(QWidget *parent)
{

}
QVariant model_dynamic_view_patient::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::BackgroundRole:
        if(index.sibling(index.row(),4).data( Qt::DisplayRole ).toString()=="Закрыт")
        {
            return qVariantFromValue(QColor(Qt::green));
        }
        else
        {
            return qVariantFromValue(QColor(Qt::white));
        }
    case Qt::DisplayRole:
        if(index.column() == 3)
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
            case 3:
                //Д-3
                return "Д-3";
                break;
            case 4:
                //Д-4
                return "Д-4";
                break;
            case 5:
                //Д-5
                return "Д-5";
                break;
            case 6:
                //Д-6
                return "Д-6";
                break;
            case 7:
                //Д-7
                return "Д-7";
                break;
            case 8:
                //АДН
                return "АДН";
                break;
            case 9:
                //АПЛ
                return "АПЛ";
                break;
            case 10:
                //ЛП
                return "ЛП";
                break;
            case 11:
                //Нет согласия
                return "Нет согласия";
                break;
            case 12:
                //К
                return "К";
                break;
            }

        }
        if(index.column() == 4)
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


    }
    return value;

}
