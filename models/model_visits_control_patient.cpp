#include "model_visits_control_patient.h"

model_visits_control_patient::model_visits_control_patient(QWidget *parent)
{

}
QVariant model_visits_control_patient::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index,role);
    switch(role){
    case Qt::BackgroundRole:
        if(index.sibling(index.row(),2).data( Qt::DisplayRole ).toDate()==QDate::currentDate() && index.sibling(index.row(),4).data( Qt::DisplayRole ).toString()=="")
        {
                return qVariantFromValue(QColor(Qt::green));
        }
        else if(index.sibling(index.row(),2).data( Qt::DisplayRole ).toDate()<QDate::currentDate() && index.sibling(index.row(),4).data( Qt::DisplayRole ).toString()=="")
        {
                return qVariantFromValue(QColor(Qt::red));
        }
        else if(index.sibling(index.row(),2).data( Qt::DisplayRole ).toDate()>QDate::currentDate())
        {
                return qVariantFromValue(QColor(Qt::white));
        }

    }
    return value;

}
