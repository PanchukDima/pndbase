#ifndef MODEL_IVALID_PATIENT_H
#define MODEL_IVALID_PATIENT_H


#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>

class model_ivalid_patient : public QSqlTableModel
{
    Q_OBJECT
public:
    model_ivalid_patient(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
};

#endif // MODEL_IVALID_PATIENT_H
