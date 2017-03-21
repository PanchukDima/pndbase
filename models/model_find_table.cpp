#include "model_find_table.h"

model_find_table::model_find_table(QWidget *parent)
{

}
QVariant model_find_table::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 4)
        {
            if(value.toBool())
            {
                return "M";
            }
            else
            {
                return "Ж";
            }
            //return value.toString()="М";
        }
        if(index.column() == 7)
        {
            return value.toString().append(" участок");
        }
        if(index.column() == 6)
        {
            if(value.toBool())
            {
                return "Да";
            }
            else
            {
                return "Нет";
            }
        }
        if(index.column() == 8)
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
        if(index.column() == 19)
        {
          switch (value.toInt()) {
            case -1:
              return "В Архиве причина не известна";
            case 0:
                return "Смерть";
            case 1:
                return "Выздоровление(стойкое улучшение)";
            case 3:
                return "Передача под наблюдение в др. учреждение";
            case 4:
                return "Выезд в другой район";
            case 5:
                return "(К) не обратился за помощью в теч. года";
            case 9:
                return  "Другие причины";
            case 10:
                return "Выбытие в стац. учр.соц.обслуживания";
            case 11:
                return  "Снятие диагноза психич. заболевания";
            case 14:
                return "(Д)отсутствие сведений в теч. длит. времени";
            case 99:
                return "Стоит на учете статус не известен";
            case 100:
                return "Первичный";
            case 101:
                return "Повторно";
            }
        }

    }
    return value ;

}
