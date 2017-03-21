#include "dialog_find_filters.h"
#include "ui_dialog_find_filters.h"

Dialog_find_filters::Dialog_find_filters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_find_filters)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    Dialog_find_filters::setWindowTitle("Фильтры поиска");
    state_widget_finds = true;
    load_list();
    ui->checkBox_check_all_area_diagnos->hide();
    ui->checkBox_all_check_diagnos_find->hide();
    ui->listWidget_area_list_diagnos->hide();
    ui->listWidget_list_diagnos->hide();
    ui->listWidget_area_list->hide();
    ui->checkBox_all_area_list->hide();
    ui->listWidget_dynamic_view_area->hide();
    ui->checkBox_all_dynamic_view_area->hide();

    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);
    this->setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint);
    connect(ui->pushButton_find,SIGNAL(clicked(bool)),SLOT(send_result()));

    connect(ui->pushButton_cancel,SIGNAL(clicked(bool)),SLOT(close()));

    //вывод на другие носители информации
    connect(ui->pushButton_print,SIGNAL(clicked(bool)),SLOT(Print_result()));
    connect(ui->pushButton_export,SIGNAL(clicked(bool)),SLOT(SaveAsCSV()));

    //сворачивание меню поиска
    connect(ui->pushButton,SIGNAL(clicked(bool)),SLOT(find_widget()));
    connect(ui->pushButton_hide_menu_diagnos,SIGNAL(clicked(bool)),SLOT(find_widget()));
    connect(ui->pushButton_show_and_hide_dynamic_view,SIGNAL(clicked(bool)),SLOT(find_widget()));
    connect(ui->pushButton_hide_invalid,SIGNAL(clicked(bool)),SLOT(find_widget()));

    connect(ui->comboBox_code_diagnos_find_diagnos,SIGNAL(activated(int)),SLOT(changes_code_diagnos(int)));
    connect(ui->comboBox_name_diagnos_find_diagnos,SIGNAL(activated(int)),SLOT(changes_name_diagnos(int)));
    connect(ui->pushButton_find_diagnos,SIGNAL(clicked(bool)),SLOT(find_filter_diagnos()));
    connect(ui->checkBox_all_check_diagnos_find,SIGNAL(stateChanged(int)),SLOT(hidden_list_diagnos(int)));

    //поиск по динамике наблюдения
    connect(ui->pushButton_find_dynamic_view,SIGNAL(clicked(bool)),SLOT(find_filter_dynamic_view()));

    //меню
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_main_filter(QPoint)));
    connect(ui->tableView_diagnos_find,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_diagnos_filter(QPoint)));
    connect(ui->tableView_show_filter_dynamic,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_dynamic_view_filter(QPoint)));

    //показывать/скрывать лист участков
    connect(ui->checkBox_show_area_list,SIGNAL(stateChanged(int)),SLOT(show_hide_area_list()));
    connect(ui->checkBox_show_area_list_diagnos,SIGNAL(stateChanged(int)),SLOT(show_hide_area_list()));
    connect(ui->checkBox_show_dynamic_view_area,SIGNAL(stateChanged(int)),SLOT(show_hide_area_list()));

    //Checked/Unchecked всех участков
    connect(ui->checkBox_all_area_list,SIGNAL(stateChanged(int)),SLOT(checked_all_area_in_list()));
    connect(ui->checkBox_all_dynamic_view_area,SIGNAL(stateChanged(int)),SLOT(checked_all_area_in_list()));
    connect(ui->checkBox_check_all_area_diagnos,SIGNAL(stateChanged(int)),SLOT(checked_all_area_in_list()));

    //invalid filter
    connect(ui->pushButton_find_invalid,SIGNAL(clicked(bool)),SLOT(find_filter_invalid()));



}

Dialog_find_filters::~Dialog_find_filters()
{
    delete ui;
}
void Dialog_find_filters::changes_code_diagnos(int index)
{
    QString id_diagnos = ui->comboBox_code_diagnos_find_diagnos->itemData(index).toString();
    ui->comboBox_name_diagnos_find_diagnos->setCurrentIndex(ui->comboBox_name_diagnos_find_diagnos->findData(id_diagnos));
}
void Dialog_find_filters::changes_name_diagnos(int index)
{
    QString id_diagnos = ui->comboBox_name_diagnos_find_diagnos->itemData(index).toString();
    ui->comboBox_code_diagnos_find_diagnos->setCurrentIndex(ui->comboBox_code_diagnos_find_diagnos->findData(id_diagnos));
}
void Dialog_find_filters::load_list()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;

    ui->listWidget_list_diagnos->hide();


    ui->comboBox_area_street->addItem("--Пусто--", "0");
    ui->comboBox_area_street->addItem("Выборгский район", "1");
    ui->comboBox_area_street->addItem("Калининский район", "2");
    ui->comboBox_sex->addItem("--Пусто--","");
    ui->comboBox_sex->addItem("Ж","false");
    ui->comboBox_sex->addItem("М","true");

    if(db.open())
    {
        query.exec("SELECT id, name FROM test.area WHERE id in(SELECT area_id FROM test.servising_area WHERE staff_id = "+obj.staff_id+") ORDER BY id;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
            QString id_status = query.value(0).toString();
            QString description = query.value(1).toString();
            QListWidgetItem * item = new QListWidgetItem();
            QListWidgetItem * item_diagnos_area = new QListWidgetItem();
            QListWidgetItem * item_dynamic_view_area = new QListWidgetItem();

            item->setText(id_status);
            item_diagnos_area->setText(id_status);
            item_dynamic_view_area->setText(id_status);

            item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item_diagnos_area->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            item_dynamic_view_area->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            item->setCheckState(Qt::Unchecked);
            item_diagnos_area->setCheckState(Qt::Unchecked);
            item_dynamic_view_area->setCheckState(Qt::Unchecked);

            ui->listWidget_area_list->addItem(item);
            ui->listWidget_area_list_diagnos->addItem(item_diagnos_area);
            ui->listWidget_dynamic_view_area->addItem(item_dynamic_view_area);

            ui->comboBox_area->addItem(description,id_status);


        }
    }
    if(db.open())
    {
        QSqlQuery query;
        query.exec("SELECT street.id, street.name FROM test.street ORDER BY street.name ASC");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {
            QString street = query.value(1).toString();
            QString code_street = query.value(0).toString();
            ui->comboBox_street->addItem(street, code_street);
        }
        ui->comboBox_street->setCurrentIndex(ui->comboBox_street->findData("25"));
    }
    if(db.open())
    {
        query.exec("SELECT diagnos.id, diagnos.name, diagnos.code, diagnos.staff_add_id FROM test.diagnos ORDER BY diagnos.code ASC;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {
            QString id_diagnos = query.value(0).toString();
            QString diagnos = query.value(1).toString();
            QString code_diagnos = query.value(2).toString();
            QListWidgetItem *item = new QListWidgetItem();

            ui->comboBox_code_diagnos_find_diagnos->addItem(code_diagnos, id_diagnos);
            ui->comboBox_name_diagnos_find_diagnos->addItem(diagnos, id_diagnos);
            item->setText(id_diagnos+";"+diagnos);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked); // AND initialize check state
            ui->listWidget_list_diagnos->addItem(item);
        }
    }

}
void Dialog_find_filters::send_result()
{
    QStringList list_query;
    QString query_str;
    if(ui->lineEdit_fname->text()!="")
    {
        list_query.append(" fname LIKE '"+ui->lineEdit_fname->text()+"%'");
    }
    if(ui->lineEdit_name->text()!="")
    {
        list_query.append(" name LIKE '"+ui->lineEdit_name->text()+"%'");
    }
    if(ui->lineEdit_oname->text()!="")
    {
        list_query.append(" mname LIKE '"+ui->lineEdit_oname->text()+"%'");
    }
    if(ui->lineEdit_date_birthday->text()!="")
    {
        list_query.append(" birthday = '"+ui->lineEdit_date_birthday->text()+"'");
    }
    if(ui->comboBox_area_street->currentData().toString()!="0")
    {
        list_query.append(" district_id = '"+ui->comboBox_area_street->currentData().toString()+"'");
    }
    if(ui->comboBox_sex->currentData().toString()!="")
    {
        list_query.append(" sex = "+ui->comboBox_sex->currentData().toString());
    }
    if(ui->comboBox_area->currentData().toString()!="0")
    {
        if(ui->checkBox_show_area_list->isChecked())
        {
            list_query.append(" area_id in("+load_isCheked_list_area()+")");
        }
        else
        {
            list_query.append(" area_id = "+ ui->comboBox_area->currentData().toString());
        }
    }
    if(ui->comboBox_street->currentData().toString()!="25")
    {
        list_query.append(" street_id ="+ui->comboBox_street->currentData().toString());
    }
    if(ui->lineEdit_passport_serial->text()!="")
    {
        list_query.append(" serial_passport = '"+ui->lineEdit_passport_serial->text()+"'");
    }
    if(ui->lineEdit_passport_number->text()!="")
    {
        list_query.append(" number_passport = '"+ ui->lineEdit_passport_number->text()+"'");
    }
    if(ui->lineEdit_house->text()!="")
    {
        list_query.append(" home = '"+ui->lineEdit_house->text()+"'");
    }
    if(ui->lineEdit_flat->text()!="")
    {
        list_query.append(" flat = '"+ui->lineEdit_flat->text()+"'");
    }
    if(ui->lineEdit_korp->text()!="")
    {
        list_query.append(" building = '"+ui->lineEdit_korp->text()+"'");
    }
    if(ui->checkBox_pt->checkState() == Qt::Checked)
    {
        list_query.append(" pt = 'true'");
    }
    else if(ui->checkBox_pt->checkState() == Qt::Unchecked)
    {
        list_query.append(" pt = 'false'");
    }

    if(ui->lineEdit_polis_number->text()!="")
    {
        list_query.append(" polis_number LIKE '"+ui->lineEdit_polis_number->text()+"%'");
    }
    if(!ui->checkBox_see_arhiv->isChecked())
    {
        list_query.append(" status in(99,100,101)");
    }

    load_table(list_query.join(" AND "));
}
void Dialog_find_filters::load_table(QString string_filter)
{
    model_find_table * model = new model_find_table();
    QString result_count;
    
    model->setTable("test.main_find_filter");
    model->setFilter(string_filter);
    model->setHeaderData(0,Qt::Horizontal,tr("Медкарта №"));
    model->setHeaderData(1,Qt::Horizontal, tr("Фамилия"));
    model->setHeaderData(2,Qt::Horizontal, tr("Имя"));
    model->setHeaderData(3,Qt::Horizontal, tr("Отчество"));
    model->setHeaderData(4,Qt::Horizontal, tr("Пол"));
    model->setHeaderData(5,Qt::Horizontal,tr("Дата\nРождения"));
    model->setHeaderData(6,Qt::Horizontal, tr("ПТ"));
    model->setHeaderData(7,Qt::Horizontal,tr("Участок"));
    model->setHeaderData(8,Qt::Horizontal, tr("Район"));
    model->setHeaderData(9,Qt::Horizontal, tr("Улица"));
    model->setHeaderData(10,Qt::Horizontal, tr("Дом"));
    model->setHeaderData(11,Qt::Horizontal, tr("Корпус"));
    model->setHeaderData(12,Qt::Horizontal, tr("Квартира"));
    model->setHeaderData(13,Qt::Horizontal, tr("Телефон"));
    model->setHeaderData(14,Qt::Horizontal, tr("Индекс"));
    model->setHeaderData(15,Qt::Horizontal, tr("Серия\nПаспорта"));
    model->setHeaderData(16,Qt::Horizontal, tr("Номер\nПаспорта"));
    model->setHeaderData(17,Qt::Horizontal, tr("Выдан"));
    model->setHeaderData(18,Qt::Horizontal, tr("Номер\nПолиса"));
    model->setHeaderData(19,Qt::Horizontal, tr("Статус"));
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(20);
    result_count.setNum(ui->tableView->model()->rowCount());
    ui->label_status->setText("Количество записей:"+result_count);

}
void Dialog_find_filters::find_filter_dynamic_view()
{
    QStringList check_dynamic_view;
    Objects_app obj;
    qDebug()<<"Dynamic_view:296";
    //delete model_dynamic;
    //model_dynamic = nullptr;
    model_dynamic = new model_dynamic_view();
    model_dynamic->setTable("test.dynamic_view_preview");
    qDebug()<<"Dynamic_view:301";
    if(ui->checkBox_d->isChecked())
    {
        check_dynamic_view.append("'1','2','3','4','5','6','7'");
    }
    if(ui->checkBox_k->isChecked())
    {
        check_dynamic_view.append("'12'");
    }
    if(ui->checkBox_lp->isChecked())
    {
        check_dynamic_view.append("'10'");
    }
    if(ui->checkBox_apl->isChecked())
    {
        check_dynamic_view.append("'9'");
    }
    if(ui->checkBox_adn->isChecked())
    {
        check_dynamic_view.append("'8'");
    }
    if(ui->checkBox_not->isChecked())
    {
        check_dynamic_view.append("'11'");
    }
    if(ui->checkBox_pt_dynamic_view->isChecked())
    {

    }


    QString filter_string;
    filter_string.append("group_disp_view in(").append(check_dynamic_view.join(",")).append(")");
    QString append_string;
    if(!ui->checkBox_show_close_dynamic->isChecked())
    {
        append_string.append(" AND status = 'f'");

    }
    if(ui->checkBox_show_dynamic_view_area->isChecked())
    {
        append_string.append(" AND area_id in("+load_isCheked_list_area()+")");
    }
    else
    {
        append_string.append(" AND area_id in("+obj.str_area_list+")");
    }
    if(!ui->checkBox_show_archiv_dynamic->isChecked())
    {
        append_string.append(" AND patient_status in(99,100,101)");
    }

    if(ui->checkBox_pt_dynamic_view->isChecked())
    {
        append_string.append(" AND pt = 'true' ");
    }
    else
    {
        append_string.append(" AND pt = 'false' ");
    }
    qDebug()<<"Dynamic_view:361";
    filter_string.append(append_string);
    model_dynamic->setFilter(filter_string);
    model_dynamic->select();
    model_dynamic->setHeaderData(0,Qt::Horizontal,tr("Номер\n динамики"));
    model_dynamic->setHeaderData(1,Qt::Horizontal,tr("Медкарта №"));
    model_dynamic->setHeaderData(2,Qt::Horizontal,tr("Статус"));
    model_dynamic->setHeaderData(3,Qt::Horizontal,tr("Удалена строка"));
    model_dynamic->setHeaderData(4,Qt::Horizontal,tr("Статус пациента"));
    model_dynamic->setHeaderData(5,Qt::Horizontal,tr("Фамилия"));
    model_dynamic->setHeaderData(6,Qt::Horizontal,tr("Имя"));
    model_dynamic->setHeaderData(7,Qt::Horizontal,tr("Отчество"));
    model_dynamic->setHeaderData(8,Qt::Horizontal,tr("Пол"));
    model_dynamic->setHeaderData(9,Qt::Horizontal,tr("Дата рождения"));
    model_dynamic->setHeaderData(10,Qt::Horizontal,tr("Группа\n наблюдения"));
    model_dynamic->setHeaderData(11,Qt::Horizontal,tr("Участок"));
    model_dynamic->setHeaderData(12,Qt::Horizontal,tr("Район"));
    model_dynamic->setHeaderData(13,Qt::Horizontal,tr("Помощь\nоказывается с:"));
    model_dynamic->setHeaderData(14,Qt::Horizontal,tr("Дата\nзакрытия\nдинамики"));
    model_dynamic->setHeaderData(15,Qt::Horizontal, tr("ПТ"));

    ui->tableView_show_filter_dynamic->setModel(model_dynamic);

    ui->tableView_show_filter_dynamic->hideColumn(0);
    ui->tableView_show_filter_dynamic->hideColumn(2);
    ui->tableView_show_filter_dynamic->hideColumn(3);
    ui->tableView_show_filter_dynamic->hideColumn(4);
    ui->tableView_show_filter_dynamic->hideColumn(10);
    ui->tableView_show_filter_dynamic->hideColumn(14);
    ui->tableView_show_filter_dynamic->hideColumn(15);


    if(ui->checkBox_show_close_dynamic->isChecked())
    {
        ui->tableView_show_filter_dynamic->hideColumn(2);
    }
    else
    {
        ui->tableView_show_filter_dynamic->hideColumn(2);
    }


}
void Dialog_find_filters::find_widget()
{
    if(state_widget_finds)
    {
        ui->frame->hide();
        ui->frame_2->hide();
        ui->frame_3->hide();
        ui->frame_4->hide();
        state_widget_finds = false;
        ui->pushButton->setText("<");
        ui->pushButton_hide_menu_diagnos->setText("<");
        ui->pushButton_show_and_hide_dynamic_view->setText("<");
        ui->pushButton_hide_invalid->setText("<");
    }
    else
    {
        ui->frame->show();
        ui->frame_2->show();
        ui->frame_3->show();
        ui->frame_4->show();
        state_widget_finds = true;
        ui->pushButton->setText(">");
        ui->pushButton_hide_menu_diagnos->setText(">");
        ui->pushButton_show_and_hide_dynamic_view->setText(">");
        ui->pushButton_hide_invalid->setText(">");
    }
}
void Dialog_find_filters::hidden_list_diagnos(int state)
{
    if(state==2)
    {
        ui->listWidget_list_diagnos->show();
    }
    else
    {
        ui->listWidget_list_diagnos->hide();
    }
}
void Dialog_find_filters::hidden_list_invalid(int state)
{

}

void Dialog_find_filters::SaveAsCSV()
{
    if(ui->tableView->isVisible())
    {
        QString file_name = QFileDialog::getSaveFileName(this,
                                                         QString::fromUtf8("Сохранить файл"),
                                                         QDir::currentPath(),
                                                         "CSV (*.csv);;All files (*.*)");
        QFile f(file_name);
        if( f.open( QIODevice::WriteOnly ) )
        {
            QTextStream ts( &f );
            QStringList strList;

            strList << "\" \"";
            for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
                strList << "\""+ui->tableView->model()->headerData(c, Qt::Horizontal).toString()+"\"";
            ts << strList.join( ";" )+"\n";

            for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
            {
                strList.clear();
                strList << "\""+ui->tableView->model()->headerData(r, Qt::Vertical).toString()+"\"";
                for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
                {
                    strList << "\""+ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
                }
                ts << strList.join( ";" )+"\n";
            }
            f.close();
        }
    }
    if(ui->tableView_diagnos_find->isVisible())
    {
        QString file_name = QFileDialog::getSaveFileName(this,
                                                         QString::fromUtf8("Сохранить файл"),
                                                         QDir::currentPath(),
                                                         "CSV (*.csv);;All files (*.*)");
        QFile f(file_name);

        if( f.open( QIODevice::WriteOnly ) )
        {
            QTextStream ts( &f );
            QStringList strList;

            strList << "\" \"";
            for( int c = 0; c < ui->tableView_diagnos_find->horizontalHeader()->count(); ++c )
                strList << "\""+ui->tableView_diagnos_find->model()->headerData(c, Qt::Horizontal).toString()+"\"";
            ts << strList.join( ";" )+"\n";

            for( int r = 0; r < ui->tableView_diagnos_find->verticalHeader()->count(); ++r )
            {
                strList.clear();
                strList << "\""+ui->tableView_diagnos_find->model()->headerData(r, Qt::Vertical).toString()+"\"";
                for( int c = 0; c < ui->tableView_diagnos_find->horizontalHeader()->count(); ++c )
                {
                    strList << "\""+ui->tableView_diagnos_find->model()->data(ui->tableView_diagnos_find->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
                }
                ts << strList.join( ";" )+"\n";
            }
            f.close();
        }
    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {
        QString file_name = QFileDialog::getSaveFileName(this,
                                                         QString::fromUtf8("Сохранить файл"),
                                                         QDir::currentPath(),
                                                         "CSV (*.csv);;All files (*.*)");
        QFile f(file_name);

        if( f.open( QIODevice::WriteOnly ) )
        {
            QTextStream ts( &f );
            QStringList strList;

            strList << "\" \"";
            for( int c = 0; c < ui->tableView_show_filter_dynamic->horizontalHeader()->count(); ++c )
                strList << "\""+ui->tableView_show_filter_dynamic->model()->headerData(c, Qt::Horizontal).toString()+"\"";
            ts << strList.join( ";" )+"\n";

            for( int r = 0; r < ui->tableView_show_filter_dynamic->verticalHeader()->count(); ++r )
            {
                strList.clear();
                strList << "\""+ui->tableView_show_filter_dynamic->model()->headerData(r, Qt::Vertical).toString()+"\"";
                for( int c = 0; c < ui->tableView_show_filter_dynamic->horizontalHeader()->count(); ++c )
                {
                    strList << "\""+ui->tableView_show_filter_dynamic->model()->data(ui->tableView_show_filter_dynamic->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
                }
                ts << strList.join( ";" )+"\n";
            }
            f.close();
        }
    }


}
void Dialog_find_filters::Print_result()
{
    QWebView *w_view = new QWebView();
    if(ui->tableView->isVisible())
    {

            QString html_str;
            QStringList strList;
            html_str.append("<table border = '1'>");
            strList << "\" \"";
            for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
                strList << "<tr><td>"+ui->tableView->model()->headerData(c, Qt::Horizontal).toString()+"\"";
            html_str.append( strList.join( "<td></td>" )+"</tr>");

            for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
            {
                strList.clear();
                strList << "\""+ui->tableView->model()->headerData(r, Qt::Vertical).toString()+"\"";
                for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
                {
                    strList << "<tr>"+ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
                html_str.append( strList.join( "<td></td>" )+"</tr>");
            }
            html_str.append("</table>");
            w_view->setHtml(html_str);
            w_view->show();

    }
    }

    if(ui->tableView_diagnos_find->isVisible())
    {

    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {

    }
}
void Dialog_find_filters::find_filter_diagnos()
{

    Objects_app Obj;
    QString area = Obj.servising_area.join(", ");
    QString fname = ui->lineEdit_fname_find_diagnos->text();
    QString name = ui->lineEdit_name_find_diagnos->text();
    QString oname = ui->lineEdit_oname_find_diagnos->text();
    QString diagnos_id = ui->comboBox_name_diagnos_find_diagnos->currentData().toString();
    QString result_count;
    QString arhive;
    QStringList string_find;
    if(oname!="")
    {
        oname[0] = oname[0].toUpper();
        string_find.append(" fname LIKE '%"+oname+"%' ");
    }
    if(fname!="")
    {
        fname[0] = fname[0].toUpper();
        string_find.append(" fname LIKE '%"+fname+"%' ");
    }
    if(name!="")
    {
        name[0] = name[0].toUpper();
        string_find.append(" fname LIKE '%"+name+"%' ");
    }
    if (!ui->checkBox_see_arhive_diagnos->isChecked())
    {
        string_find.append(" status in(99,100,101) ");
    }
    string_find.append(" delete_row = 'false' ");
    string_find.append(" diagnos_id = "+diagnos_id);
    model_diagnos_filter *model = new model_diagnos_filter;
    model->setTable("test.diagnos_filter");
    model->setFilter(string_find.join(" AND "));
    model->select();
    model->setHeaderData(0,Qt::Horizontal,tr("Медкарта №"));
    model->setHeaderData(4,Qt::Horizontal,tr("Фамилия"));
    model->setHeaderData(5,Qt::Horizontal,tr("Имя"));
    model->setHeaderData(6,Qt::Horizontal,tr("Отчество"));
    model->setHeaderData(7,Qt::Horizontal,tr("Пол"));
    model->setHeaderData(8,Qt::Horizontal,tr("Дата\nРождения"));
    model->setHeaderData(9,Qt::Horizontal,tr("Участок"));
    model->setHeaderData(10,Qt::Horizontal,tr("Район"));
    model->setHeaderData(11,Qt::Horizontal,tr("Дата\nпостановки\nдиагноза"));
    ui->tableView_diagnos_find->setModel(model);
    ui->tableView_diagnos_find->hideColumn(1);
    ui->tableView_diagnos_find->hideColumn(2);
    ui->tableView_diagnos_find->hideColumn(3);
    result_count.setNum(ui->tableView_diagnos_find->model()->rowCount());
    ui->label_find_result->setText("Всего найдено: "+result_count);


}
void Dialog_find_filters::context_menu_main_filter(QPoint pos)
{
    QMenu *menu = new QMenu;
    Objects_app obj;
    menu->addAction("Просмотр", this, SLOT(view_patient_info()))->setEnabled(true);
    menu->addAction("Изменить", this, SLOT(edit_patient_info()))->setEnabled(obj.rights_user[19]);
    menu->addAction("Удалить", this, SLOT(del_patient()))->setEnabled(obj.rights_user[20]);
    menu->exec(ui->tableView->mapToGlobal(pos));
}
void Dialog_find_filters::context_menu_diagnos_filter(QPoint pos)
{
    QMenu *menu = new QMenu;
    Objects_app obj;
//    menu->addAction("Просмотр", this, SLOT(view_patient_info()))->setEnabled(true);
//    menu->addAction("Изменить", this, SLOT(edit_patient_info()))->setEnabled(obj.rights_user[19]);
//    menu->addAction("Удалить", this, SLOT(del_patient()))->setEnabled(obj.rights_user[20]);
    menu->addAction("Показать диагноз", this, SLOT(show_diagnos_in_main_table()));
    menu->exec(ui->tableView_diagnos_find->mapToGlobal(pos));
}
void Dialog_find_filters::context_menu_dynamic_view_filter(QPoint pos)
{
    QMenu *menu = new QMenu;
    Objects_app obj;
    menu->addAction("Просмотр", this, SLOT(view_patient_info()))->setEnabled(true);
    menu->addAction("Изменить", this, SLOT(edit_patient_info()))->setEnabled(obj.rights_user[19]);
    menu->addAction("Удалить", this, SLOT(del_patient()))->setEnabled(obj.rights_user[20]);
    menu->exec(ui->tableView_show_filter_dynamic->mapToGlobal(pos));
}
void Dialog_find_filters::view_patient_info()
{
    Dialog_patient dialog;
    Objects_app obj;
    int medcard_id;
    if(ui->tableView->isVisible())
    {
        int selected_tables = ui->tableView->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView->currentIndex().row();
            medcard_id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toInt();
        }
    }
    if(ui->tableView_diagnos_find->isVisible())
    {
        int selected_tables = ui->tableView_diagnos_find->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_diagnos_find->currentIndex().row();
            medcard_id = ui->tableView_diagnos_find->model()->index(row,0).data(Qt::DisplayRole).toInt();
        }
    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {
        int selected_tables = ui->tableView_show_filter_dynamic->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_show_filter_dynamic->currentIndex().row();
            medcard_id = ui->tableView_show_filter_dynamic->model()->index(row,1).data(Qt::DisplayRole).toInt();
        }
    }
    dialog.setParam(2,medcard_id,obj.rights_user[29]);
    dialog.exec();
}
void Dialog_find_filters::edit_patient_info()
{
    Dialog_patient dialog;
    Objects_app obj;
    int medcard_id;
    if(ui->tableView->isVisible())
    {
        int selected_tables = ui->tableView->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView->currentIndex().row();
            medcard_id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toInt();
        }
    }
    if(ui->tableView_diagnos_find->isVisible())
    {
        int selected_tables = ui->tableView_diagnos_find->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_diagnos_find->currentIndex().row();
            medcard_id = ui->tableView_diagnos_find->model()->index(row,0).data(Qt::DisplayRole).toInt();
        }
    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {
        int selected_tables = ui->tableView_show_filter_dynamic->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_show_filter_dynamic->currentIndex().row();
            medcard_id = ui->tableView_show_filter_dynamic->model()->index(row,1).data(Qt::DisplayRole).toInt();
        }
    }
    dialog.setParam(1,medcard_id,obj.rights_user[29]);
    dialog.exec();
}
void Dialog_find_filters::del_patient()
{
    if(ui->tableView->isVisible())
    {

    }
    if(ui->tableView_diagnos_find->isVisible())
    {

    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {

    }
}
void Dialog_find_filters::show_hide_area_list()
{
    if(ui->tableView->isVisible())
    {
        if(ui->checkBox_show_area_list->isChecked())
        {
            ui->comboBox_area->hide();
            ui->label_3->hide();
            ui->checkBox_all_area_list->show();
            ui->listWidget_area_list->show();
        }
        else
        {
            ui->comboBox_area->show();
            ui->label_3->show();
            ui->checkBox_all_area_list->hide();
            ui->listWidget_area_list->hide();
        }
    }
    if(ui->tableView_diagnos_find->isVisible())
    {
        if(ui->checkBox_show_area_list_diagnos->isChecked())
        {
            ui->listWidget_area_list_diagnos->show();
            ui->checkBox_check_all_area_diagnos->show();
        }
        else
        {
            ui->listWidget_area_list_diagnos->hide();
            ui->checkBox_check_all_area_diagnos->hide();

        }
    }
    if(ui->tableView_show_filter_dynamic->isVisible())
    {
        if(ui->checkBox_show_dynamic_view_area->isChecked())
        {
            ui->listWidget_dynamic_view_area->show();
            ui->checkBox_all_dynamic_view_area->show();
        }
        else
        {
            ui->listWidget_dynamic_view_area->hide();
            ui->checkBox_all_dynamic_view_area->hide();
        }
    }


}
void Dialog_find_filters::checked_all_area_in_list()
{

    if(ui->checkBox_all_area_list->isChecked())
    {
        for(int i = 0; ui->listWidget_area_list->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_area_list->item(i);
            if(item->checkState()==Qt::Unchecked)
            {
                item->setCheckState(Qt::Checked);
            }
        }
    }
    else
    {
        for(int i = 0; ui->listWidget_area_list->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_area_list->item(i);
            if(item->checkState()==Qt::Checked)
            {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    if(ui->checkBox_check_all_area_diagnos->isChecked())
    {
        for(int i = 0; ui->listWidget_area_list_diagnos->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_area_list_diagnos->item(i);
            if(item->checkState()==Qt::Unchecked)
            {
                item->setCheckState(Qt::Checked);
            }
        }
    }
    else
    {
        for(int i = 0; ui->listWidget_area_list_diagnos->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_area_list_diagnos->item(i);
            if(item->checkState()==Qt::Checked)
            {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
    if(ui->checkBox_all_dynamic_view_area->isChecked())
    {
        for(int i = 0; ui->listWidget_dynamic_view_area->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_dynamic_view_area->item(i);
            if(item->checkState()==Qt::Unchecked)
            {
                item->setCheckState(Qt::Checked);
            }
        }
    }
    else
    {
        for(int i = 0; ui->listWidget_dynamic_view_area->count()>i; i++)
        {
            QListWidgetItem* item = ui->listWidget_dynamic_view_area->item(i);
            if(item->checkState()==Qt::Checked)
            {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }
}
void Dialog_find_filters::find_filter_invalid()
{
    Objects_app obj;
    model_invalid = new model_filter_invalid();
    model_invalid->setTable("test.invalid_filter");
    QStringList filter_str;
    if(ui->checkBox_group_inv->isChecked())
    {
        QStringList group_inv;
        if(ui->checkBox_inv_1->isChecked())
        {
            group_inv.append(" group_inv = '0' ");
        }
        if(ui->checkBox_inv_2->isChecked())
        {
            group_inv.append(" group_inv = '1' ");
        }
        if(ui->checkBox_inv_3->isChecked())
        {
            group_inv.append(" group_inv = '2' ");
        }
        group_inv.append(" group_inv = '' ");
        group_inv.append(" group_inv = '-1' ");
        filter_str.append("("+group_inv.join(" OR ")+")");
    }
    if(ui->checkBox_date_install->isChecked())
    {
        filter_str.append(" date_peresmotra >='"+ui->dateEdit_is_install->date().toString("dd.MM.yyyy")+"' AND date_peresmotra <='"+ui->dateEdit_to_install->date().toString("dd.MM.yyyy")+"' ");
    }
    if(ui->checkBox_date_pere->isChecked())
    {
        filter_str.append(" srok_ocherednogo_pereosvidet >='"+ui->dateEdit_is_pere->date().toString("dd.MM.yyyy")+"' AND srok_ocherednogo_pereosvidet <='"+ui->dateEdit_to_pere->date().toString("dd.MM.yyyy")+"' ");
    }
    if(!ui->checkBox_see_invalid_arhiv->isChecked())
    {
        filter_str.append(" status in(99,100,101) ");
    }
    if(!ui->checkBox_bs->isChecked())
    {
        filter_str.append(" bs = false ");
    }
    if(ui->checkBox_dubl->isChecked())
    {
        filter_str.append(" medcard_id in(SELECT medcard_id FROM test.invalid_patient WHERE delete_row = false group by medcard_id having count(*)>1) ");
    }
    filter_str.append(" area_id in ("+obj.servising_area.join(",")+") ");
    qDebug()<<filter_str.join(" AND ");
    model_invalid->setFilter(filter_str.join(" AND "));
    qDebug()<<filter_str.join(" AND ");
    model_invalid->select();
    model_invalid->setHeaderData(0,Qt::Horizontal,tr("Медкарта №"));
    model_invalid->setHeaderData(1,Qt::Horizontal,tr("Фамилия"));
    model_invalid->setHeaderData(2,Qt::Horizontal,tr("Имя"));
    model_invalid->setHeaderData(3,Qt::Horizontal,tr("Отчетство"));
    model_invalid->setHeaderData(4,Qt::Horizontal,tr("Группа инвалидности"));
    model_invalid->setHeaderData(5,Qt::Horizontal,tr("Участок"));
    model_invalid->setHeaderData(6,Qt::Horizontal,tr("Дата пересмотра"));
    model_invalid->setHeaderData(7,Qt::Horizontal,tr("Дата переосвидетельсвования"));
    model_invalid->setHeaderData(8,Qt::Horizontal,tr("Статус"));
    model_invalid->setHeaderData(9,Qt::Horizontal,tr("Место работы"));
    model_invalid->setHeaderData(10,Qt::Horizontal,tr("Первичный"));
    model_invalid->setHeaderData(11,Qt::Horizontal,tr("Степень утраченой работоспособности"));
    model_invalid->setHeaderData(12,Qt::Horizontal,tr("Изменил"));
    model_invalid->setHeaderData(13,Qt::Horizontal,tr("Другие рекомендации"));
    model_invalid->setHeaderData(14,Qt::Horizontal,tr("Дубл"));
    model_invalid->setHeaderData(15,Qt::Horizontal,tr("Диагноз"));
    model_invalid->setHeaderData(16,Qt::Horizontal,tr("Инвалид детства"));
    model_invalid->setHeaderData(17,Qt::Horizontal,tr("Бессрочно"));
    model_invalid->setHeaderData(18,Qt::Horizontal,tr("Из другого"));
    model_invalid->setHeaderData(19,Qt::Horizontal,tr("Признан трудоспособным"));
    ui->tableView_invalid->setModel(model_invalid);
}


QString Dialog_find_filters::load_isCheked_list_area()
{
    if(ui->tableView->isVisible())
    {
        QStringList list_area;
        for(int i = 0; ui->listWidget_area_list->count()>i; i++)
        {
            try{
                QListWidgetItem* item = ui->listWidget_area_list->item(i);
                if(item->checkState()==Qt::Checked)
                {
                    list_area.append(item->text());
                }
            }
            catch(...)
            {
                qDebug()<<"Error,Exception";
            }
        }
        return list_area.join(",");
    }
    else if(ui->tableView_diagnos_find->isVisible())
    {
        QStringList list_area;
        for(int i = 0; ui->listWidget_area_list_diagnos->count()>i; i++)
        {
            try{
                QListWidgetItem* item = ui->listWidget_area_list_diagnos->item(i);
                if(item->checkState()==Qt::Checked)
                {
                    list_area.append(item->text());
                }
            }
            catch(...)
            {
                qDebug()<<"Error,Exception";
            }
        }
        return list_area.join(",");
    }
    else if(ui->tableView_show_filter_dynamic->isVisible())
    {
        QStringList list_area;
        for(int i = 0; ui->listWidget_dynamic_view_area->count()>i; i++)
        {
            try{
                QListWidgetItem* item = ui->listWidget_dynamic_view_area->item(i);
                if(item->checkState()==Qt::Checked)
                {
                    list_area.append(item->text());
                }
            }
            catch(...)
            {
                qDebug()<<"Error,Exception";
            }
        }
        return list_area.join(",");
    }
    else
    {
        return "";
    }
}
void Dialog_find_filters::show_diagnos_in_main_table()
{
    QString medcard_id;
    QString id;
    int selected_tables = ui->tableView_diagnos_find->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_diagnos_find->currentIndex().row();
        medcard_id = ui->tableView_diagnos_find->model()->index(row,1).data(Qt::DisplayRole).toString();
        id = ui->tableView_diagnos_find->model()->index(row,0).data(Qt::DisplayRole).toString();
        emit diagnos(medcard_id,id,1);
        Dialog_find_filters::showMinimized();
    }
}
void Dialog_find_filters::show_dynamic_view_in_main_table()
{

}
void Dialog_find_filters::show_main_filter_in_main_table()
{

}
