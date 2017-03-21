#include "dialog_visiting_control.h"
#include "ui_dialog_visiting_control.h"

Dialog_visiting_control::Dialog_visiting_control(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_visiting_control)
{

    ui->setupUi(this);
    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);
    this->setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint);
    this->setModal(false);
    load_model_sql();

    ui->dateEdit_from->setDate(QDate::currentDate());
    ui->dateEdit_to->setDate(QDate::currentDate());
    connect(ui->pushButton_green,SIGNAL(clicked(bool)),SLOT(view_green()));
    connect(ui->pushButton_gray,SIGNAL(clicked(bool)),SLOT(view_gray()));
    connect(ui->pushButton_red,SIGNAL(clicked(bool)),SLOT(view_red()));
    connect(ui->pushButton_view_all,SIGNAL(clicked(bool)),SLOT(view_all()));
    connect(ui->pushButton_find,SIGNAL(clicked(bool)),SLOT(view_all()));
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_table(QPoint)));

}

Dialog_visiting_control::~Dialog_visiting_control()
{
    delete ui;
}
void Dialog_visiting_control::load_model_sql()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    model = new model_visiting_control_all();
    model->setTable("test.visiting_big_table");
    model->setFilter("area_id in("+obj.servising_area.join(",")+")");
    model->select();


    model->setHeaderData(0,Qt::Horizontal,tr("id"));
    model->setHeaderData(1,Qt::Horizontal,tr("Медкарта №"));
    model->setHeaderData(2,Qt::Horizontal,tr("Фамилия"));
    model->setHeaderData(3,Qt::Horizontal,tr("Имя"));
    model->setHeaderData(4,Qt::Horizontal,tr("Отчество"));
    model->setHeaderData(5,Qt::Horizontal,tr("Участок"));
    model->setHeaderData(6,Qt::Horizontal,tr("Группа наблюдения"));
    model->setHeaderData(7,Qt::Horizontal,tr("Дата\nназначеного\nпосещения"));
    model->setHeaderData(8,Qt::Horizontal,tr("Кто назначил"));
    model->setHeaderData(9,Qt::Horizontal,tr("Дата посещения"));



    ui->tableView->setModel(model);

    ui->tableView->hideColumn(0);

    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Stretch);
}
void Dialog_visiting_control::view_green()
{
    Objects_app obj;
    ui->checkBox_filter_to_date->hide();
    ui->pushButton_find->hide();
    ui->dateEdit_from->hide();
    ui->dateEdit_to->hide();
    ui->label_from->hide();
    ui->label_to->hide();
    model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_assigned = '"+QDate::currentDate().toString("dd.MM.yyyy")+"' AND date_presence is null AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    if(model->lastError().isValid())
    {
        qDebug()<<model->lastError();
    }
}
void Dialog_visiting_control::view_gray()
{
    Objects_app obj;
    ui->checkBox_filter_to_date->show();
    ui->pushButton_find->show();
    ui->dateEdit_from->show();
    ui->dateEdit_to->show();
    ui->label_from->show();
    ui->label_to->show();
    if(ui->checkBox_filter_to_date->isChecked())
    {
        model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_assigned >= '"+ui->dateEdit_from->date().toString("dd.MM.yyyy")+"'AND date_presence is not null AND date_assigned <= '"+ui->dateEdit_to->date().toString("dd.MM.yyyy")+"' AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
    else
    {
        model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_presence is not null AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
}
void Dialog_visiting_control::view_red()
{
    Objects_app obj;
    ui->checkBox_filter_to_date->show();
    ui->pushButton_find->show();
    ui->dateEdit_from->show();
    ui->dateEdit_to->show();
    ui->label_from->show();
    ui->label_to->show();
    if(ui->checkBox_filter_to_date->isChecked())
    {
        if(ui->dateEdit_to->date()<=QDate::currentDate())
        {
            QMessageBox::warning(this,"Ошибка", "Дата должна быть меньше текущей");
            return;
        }
        model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_assigned >= '"+ui->dateEdit_from->date().toString("dd.MM.yyyy")+"' AND date_assigned <= '"+ui->dateEdit_to->date().toString("dd.MM.yyyy")+"' AND date_presence is null AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
    else
    {
    model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_assigned < '"+QDate::currentDate().toString("dd.MM.yyyy")+"' AND date_presence is null AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
}
void Dialog_visiting_control::view_all()
{
    Objects_app obj;
    ui->checkBox_filter_to_date->show();
    ui->pushButton_find->show();
    ui->dateEdit_from->show();
    ui->dateEdit_to->show();
    ui->label_from->show();
    ui->label_to->show();
    if(ui->checkBox_filter_to_date->isChecked())
    {
        model->setFilter("area_id in("+obj.servising_area.join(",")+") AND date_assigned >= '"+ui->dateEdit_from->date().toString("dd.MM.yyyy")+"' AND date_assigned <= '"+ui->dateEdit_to->date().toString("dd.MM.yyyy")+"' AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
    else
    {
        model->setFilter("area_id in("+obj.servising_area.join(",")+") AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
    }
}
void Dialog_visiting_control::context_menu_table(QPoint pos)
{
    QDate date_visiting;
    QDate date_visiting_ok;
    bool redis;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        date_visiting = ui->tableView->model()->index(row,7).data(Qt::DisplayRole).toDate();
        date_visiting_ok = ui->tableView->model()->index(row,9).data(Qt::DisplayRole).toDate();
    }
    if(date_visiting_ok.isNull())
    {
        if(date_visiting<QDate::currentDate())
        {
            redis = true;
        }
        else
        {
            redis = false;
        }
    }
    else
    {
        redis = false;
    }
    QMenu *menu = new QMenu;
    menu->addAction("Показать посещение", this, SLOT(selected_medcard_id()));
    menu->addAction("Добавить причину не явки", this, SLOT(add_cause()))->setEnabled(redis);
    menu->exec(ui->tableView->mapToGlobal(pos));
}
void Dialog_visiting_control::add_cause()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("UPDATE ");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
        }
    }
}
void Dialog_visiting_control::selected_medcard_id()
{
    QString medcard_id;
    QString id;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        medcard_id = ui->tableView->model()->index(row,1).data(Qt::DisplayRole).toString();
        id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        emit get_medcard_id(medcard_id,id,0);
        Dialog_visiting_control::showMinimized();
    }
}

