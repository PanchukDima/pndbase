#include "model_visiting_control_all.h"

model_visiting_control_all::model_visiting_control_all(QWidget *parent)
{

}
QVariant model_visiting_control_all::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::BackgroundRole:
        if(index.sibling(index.row(),7).data( Qt::DisplayRole ).toDate()==QDate::currentDate() && index.sibling(index.row(),9).data( Qt::DisplayRole ).toString()=="")
        {
                return qVariantFromValue(QColor(Qt::green));
        }
        else if(index.sibling(index.row(),7).data( Qt::DisplayRole ).toDate()<QDate::currentDate() && index.sibling(index.row(),9).data( Qt::DisplayRole ).toString()=="")
        {
                return qVariantFromValue(QColor(Qt::red));
        }
        else if(index.sibling(index.row(),7).data( Qt::DisplayRole ).toDate()>QDate::currentDate())
        {
                return qVariantFromValue(QColor(Qt::white));
        }
        else if(index.sibling(index.row(),9).data( Qt::DisplayRole ).toDate().isValid())
        {
                return qVariantFromValue(QColor(Qt::gray));
        }

    case Qt::DisplayRole:
        if(index.column() == 6)
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
    }
    return value;

}
