#ifndef DIALOG_CHAT_H
#define DIALOG_CHAT_H

#include <QDialog>
#include <QtNetwork>
#include <QUdpSocket>
#include <QSqlTableModel>

#include "mainwindow.h"

namespace Ui {
class Dialog_chat;
}

class Dialog_chat : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_chat(QWidget *parent = 0);
    ~Dialog_chat();
    QSqlTableModel *model_staff;
    QSqlTableModel *model_groups;
    QSqlTableModel *model_chat;
    QString id_message;
    int select_type_message;



private:
    Ui::Dialog_chat *ui;
public slots:
    void loadMessages();
    void sendMessages();
    void loadGroups();
    void loadStaff();
    void init_start();

};

#endif // DIALOG_CHAT_H
