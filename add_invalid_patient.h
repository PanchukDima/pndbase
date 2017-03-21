#ifndef ADD_INVALID_PATIENT_H
#define ADD_INVALID_PATIENT_H

#include <QObject>

class add_invalid_patient : public QObject
{
    Q_OBJECT
public:
    explicit add_invalid_patient(QObject *parent = 0);

signals:

public slots:
    void add_I_invalid();
    void add_II_invalid();
    void add_III_invalid();

};

#endif // ADD_INVALID_PATIENT_H
