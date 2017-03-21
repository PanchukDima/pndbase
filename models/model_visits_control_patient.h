#ifndef MODEL_VISITS_CONTROL_PATIENT_H
#define MODEL_VISITS_CONTROL_PATIENT_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QDate>

class model_visits_control_patient : public QSqlTableModel
{
    Q_OBJECT
public:
        model_visits_control_patient(QWidget *parent = 0);
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_VISITS_CONTROL_PATIENT_H
