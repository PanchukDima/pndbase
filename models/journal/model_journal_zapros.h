#ifndef MODEL_JOURNAL_ZAPROS_H
#define MODEL_JOURNAL_ZAPROS_H

#include <QSqlTableModel>
#include <QObject>
#include <QColor>
#include <QBrush>
#include <QDate>

class model_journal_zapros : public QSqlTableModel
{
    Q_OBJECT
public:
    model_journal_zapros(QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
signals:

public slots:
};

#endif // MODEL_JOURNAL_ZAPROS_H
