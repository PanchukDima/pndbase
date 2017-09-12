#ifndef MAINWINDOW_POLICE_H
#define MAINWINDOW_POLICE_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDate>
#include "thread_load_data.h"
#include <QThread>

namespace Ui {
class MainWindow_police;
}

class MainWindow_police : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_police(QWidget *parent = 0);
    ~MainWindow_police();
    thread_load_data * thread_1;

private:
    Ui::MainWindow_police *ui;
public slots:
    void load_data(QString);
    void start_generate();
    void start_thread(QString file_name);
    void change_status(int);
    void change_progres(int);
    void change_progress_max(int);
    void cancel_other();

};

#endif // MAINWINDOW_POLICE_H
