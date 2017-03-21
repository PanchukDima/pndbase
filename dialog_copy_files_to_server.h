#ifndef DIALOG_COPY_FILES_TO_SERVER_H
#define DIALOG_COPY_FILES_TO_SERVER_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class Dialog_copy_files_to_server;
}

class Dialog_copy_files_to_server : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_copy_files_to_server(QWidget *parent = 0);
    ~Dialog_copy_files_to_server();
    QString global_id;
    QString global_staff;
    QString path;

private:
    Ui::Dialog_copy_files_to_server *ui;
public slots:
    void setParam(QString,QString);
    void files_load();
    void files_send();
};

#endif // DIALOG_COPY_FILES_TO_SERVER_H
