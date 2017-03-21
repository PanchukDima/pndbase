#include "model_filter_invalid.h"

model_filter_invalid::model_filter_invalid(QWidget *parent)
{

}

QVariant model_filter_invalid::data(const QModelIndex &index, int role) const
{
    // Qt::DisplayRole - данные для отображения ячейки таблицы
    // Qt::EditRole    - данные для режима редактирования
    // Qt::TextColorRole - цвет текста
    // Qt::TextAlignmentRole - выравнивание
    // Qt::FontRole - параметры шрифта
    // Qt::BackgroundColorRole - цвет фона ячейки
    // Qt::CheckStateRole - отображение QCheckBox
    // Qt::SizeHintRole - предпочитаемый размер ячейки

    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 4)
        {
            switch (value.toInt()) {
            case -1:
                return "Нет группы";
            case 0:
                return "I";
            case 1:
                return "II";
            case 2:
                return "III";
            }
        }
        if(index.column() == 5)
        {
            return value.toString().append(" участок");
        }
        if(index.column() == 8)
        {
            switch (value.toInt()) {
            case 99:
                return "не известно";
            case 100:
                return "Первичный";
            case 101:
                return "Повторно";
            case -1:
                return "";
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
                return "Другие причины";
            case 10:
                return "Выбытие в стац. учр.соц.обслуживания";
            case 11:
                return "Снятие диагноза психич. заболевания";
            case 14:
                return "(Д)отсутствие сведений в теч. длит. времени";
            }
        }
        if(index.column() == 10)
        {
            if(value.toString()== "true")
            {
                return "Да";
            }
            else if(value.toString()== "false")
            {
                return "Нет";
            }
            else if(value.toString()== "")
            {
                return "";
            }
        }
        if(index.column() == 16)
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
        if(index.column() == 17)
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
        if(index.column() == 18)
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
        if(index.column() == 19)
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

    }
    return value;
}
