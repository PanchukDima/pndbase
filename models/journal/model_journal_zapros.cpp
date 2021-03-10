#include "model_journal_zapros.h"

model_journal_zapros::model_journal_zapros(QWidget *parent)
{

}
QVariant model_journal_zapros::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::DisplayRole:
        if(index.column() == 15)
        {
            switch (value.toInt()) {
            case 1:
                return "Караулов О.А.";
                break;
            case 2:
                return "Саренок А.Н.";
                break;
            case 3:
                return "Николаева Е.Н.";
                break;
            case 4:
                return "Другое";
                break;
            }
        }

    }
    return  QSqlTableModel::data(index, role);
}
