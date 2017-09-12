#ifndef MAINWINDOW_LIST_MILITARY_H
#define MAINWINDOW_LIST_MILITARY_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>


namespace Ui {
class MainWindow_list_military;
}

class MainWindow_list_military : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_list_military(QWidget *parent = 0);
    ~MainWindow_list_military();
    QSqlTableModel *model;

private:
    Ui::MainWindow_list_military *ui;
public slots:
    void set_param_table();
    void load_table();
    void export_csv();
    void find_military();
};

#endif // MAINWINDOW_LIST_MILITARY_H
