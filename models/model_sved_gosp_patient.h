#ifndef MODEL_SVED_GOSP_PATIENT_H
#define MODEL_SVED_GOSP_PATIENT_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QDate>

class model_sved_gosp_patient : public QSqlTableModel
{
    Q_OBJECT
public:
    model_sved_gosp_patient(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


signals:

public slots:
};

#endif // MODEL_SVED_GOSP_PATIENT_H
