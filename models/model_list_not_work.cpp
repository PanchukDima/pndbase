#include "model_list_not_work.h"

model_list_not_work::model_list_not_work(QObject *parent)
{

}
QVariant model_list_not_work::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 10 || index.column() == 9)
        {
            switch (value.toInt()) {
            case 1:
                //Д
                return "ПТ";
                break;
            case 2:
                //Д-1
                return "Участковая служба";
                break;
            case 3:
                //Д-2
                return "Дневной стационар";
                break;
            case 4:
                return "Больница";
                break;
        }
        }


    }
    return value;
}
