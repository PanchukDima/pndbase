#include "dialog_patient.h"
#include "ui_dialog_patient.h"

Dialog_patient::Dialog_patient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_patient)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    this->setModal(false);
    put_all_settings();
    text_change = false;
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->pushButton_apply,SIGNAL(clicked()),SLOT(apply_send_data_sql()));
    //connect(ui->lineEdit_work_post,SIGNAL(textChanged(QString)),SLOT(get_work_post()));
    connect(ui->pushButton_ok,SIGNAL(clicked()),SLOT(apply_send_data_sql()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),SLOT(button_cancel()));
    connect(ui->pushButton_add_street,SIGNAL(clicked()),SLOT(add_street()));

    connect(ui->lineEdit_fname,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_fname()));
    connect(ui->lineEdit_name,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_name()));
    //connect(ui->lineEdit_oname,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_mname()));
    connect(ui->lineEdit_serial_passport,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_serial_passport()));
    connect(ui->lineEdit_number_passport,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_number_passport()));
    //connect(ui->lineEdit_house,SIGNAL(textEdited(QString)),SLOT(set_default_color_lineedit_house()));

    connect(ui->checkBox_ds_end_state,SIGNAL(toggled(bool)),SLOT(change_state_ds_end(bool)));
    connect(ui->lineEdit_date_start,SIGNAL(textChanged(QString)),SLOT(point_add_ds_start(QString)));
    connect(ui->lineEdit_date_birtday,SIGNAL(textChanged(QString)),SLOT(point_add_birtday(QString)));
    connect(ui->lineEdit_date_end,SIGNAL(textChanged(QString)),SLOT(point_add_ds_stop(QString)));
    connect(ui->pushButton_add_history,SIGNAL(clicked(bool)),SLOT(add_histry()));
    connect(ui->comboBox_area,SIGNAL(currentIndexChanged(int)),SLOT(set_default_color_combox_area()));
    connect(ui->comboBox_area_street,SIGNAL(currentIndexChanged(int)),SLOT(set_default_color_combox_area_street()));
    connect(ui->comboBox_sex,SIGNAL(currentIndexChanged(int)),SLOT(set_default_color_combox_sex()));
    
    //редактор текста

    //connect(ui->textEdit_document,SIGNAL(selectionChanged()),SLOT(select_text_document()));
    connect(ui->comboBox_end,SIGNAL(currentIndexChanged(QString)),SLOT(change_combox_document()));
    connect(ui->comboBox_document,SIGNAL(currentIndexChanged(QString)),SLOT(load_documents()));

    connect(ui->pushButton_document_edit,SIGNAL(clicked(bool)),SLOT(select_document_on_table()));
    connect(ui->pushButton_document_create,SIGNAL(clicked(bool)),SLOT(create_document()));
    connect(ui->pushButton_document_delete,SIGNAL(clicked(bool)),SLOT(delete_document()));
    //ui->comboBox_marital_status->setEnabled(true);
    connect(ui->pushButton_edit_date_create_medcard,SIGNAL(clicked(bool)),SLOT(edit_date_create_medcard()));

    //комментарии
    connect(ui->pushButton_add_comments,SIGNAL(clicked(bool)),SLOT(add_comments()));
    connect(ui->tableView_comments_patient,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_comments(QPoint)));
    connect(ui->pushButton_del_uchet,SIGNAL(clicked(bool)),SLOT(del_uchet()));

    apply_rights();
}

Dialog_patient::~Dialog_patient()
{
    delete ui;
}

void Dialog_patient::apply_rights()
{
    Objects_app obj;
    ui->pushButton_del_uchet->setEnabled(obj.rights_user[44]);
}

void Dialog_patient::select_changes_text_document(QTextCharFormat format)
{

//    ui->pushButton_bold->setChecked(format.font().bold());
//    ui->pushButton_italic->setChecked(format.font().italic());
//    ui->pushButton_p->setChecked(format.font().underline());
//    ui->fontComboBox_font_family->setCurrentIndex(ui->fontComboBox_font_family->findText(QFontInfo(format.font()).family()));
//    ui->spinBox_font_size->setValue(format.font().pointSize());
}
void Dialog_patient::change_combox_document()
{
    QSqlDatabase db = QSqlDatabase::database();
    qDebug()<<ui->comboBox_end->currentData().toString();
    QString id_end = ui->comboBox_end->currentData().toString();
    ui->comboBox_document->clear();
    if(db.open())
    {
        qDebug()<<"load list documents";
        QSqlQuery query;
        query.exec("SELECT id, description FROM test.document WHERE id_end = '"+id_end+"'");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
                    QString id_status = query.value(0).toString();
                    QString description = query.value(1).toString();
                    ui->comboBox_document->addItem(description,id_status);
                }
        query.clear();

    }
}
void Dialog_patient::load_documents()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString id_document = ui->comboBox_document->currentData().toString();
    if(db.open())
    {
        QSqlTableModel *model = new QSqlTableModel();
        model->setTable("test.text_info_patients");
        model->setFilter("doc_id='"+id_document+"' AND delete_row = false AND medcard_id = "+global_id_str);
        model->setHeaderData(0,Qt::Horizontal,tr("Номер документа\nв базе"));
        model->setHeaderData(3,Qt::Horizontal,tr("Дата\n последнего\n изменения"));
        model->select();

        ui->tableView_documents_list->setModel(model);
        ui->tableView_documents_list->hideColumn(1);
        ui->tableView_documents_list->hideColumn(2);
        ui->tableView_documents_list->hideColumn(4);
        ui->tableView_documents_list->hideColumn(5);
        ui->tableView_documents_list->hideColumn(6);
        ui->tableView_documents_list->hideColumn(7);
        //ui->textEdit_document->clear();
    }
    else
    {
        qDebug()<<"server lost";
    }
}
void Dialog_patient::select_document_on_table()
{
    qDebug()<<"load_text";
    Objects_app obj;
    Window_Document_Editor *WDE = new Window_Document_Editor();
    connect(WDE,SIGNAL(close_document()),this,SLOT(show()));
        int selected_tables = ui->tableView_documents_list->currentIndex().row();
        if (selected_tables >= 0)
        {
            int row = ui->tableView_documents_list->currentIndex().row();
            QString id = ui->tableView_documents_list->model()->index(row,0).data(Qt::DisplayRole).toString();
            switch (obj.staff_position) {
            default:
                            WDE->setFocus();
                            WDE->activateWindow();
                            WDE->raise();
                            WDE->setWindowModality(Qt::WindowModal);
                            WDE->showMaximized();
                            WDE->open_document(1,id);
                break;

            }
        }
}

void Dialog_patient::open_new_document(QString id)
{
    Window_Document_Editor *WDE = new Window_Document_Editor();
    connect(WDE,SIGNAL(close_document()),this,SLOT(show()));
    WDE->setFocus();
    WDE->activateWindow();
    WDE->raise();
    WDE->setWindowModality(Qt::WindowModal);
    WDE->showMaximized();
    WDE->open_document(1,id);
}

void Dialog_patient::show_dialog_add_patient()
{
    qDebug()<<"Start show";
    Dialog_patient::show();
}


void Dialog_patient::create_document()
{
    QSqlDatabase db = QSqlDatabase::database();
    QString id_document = ui->comboBox_document->currentData().toString();
    if(db.open())
    {
        QSqlQuery query;
        query.exec("INSERT INTO test.text_info_patients(medcard_id, date_creat, staff_add_id, delete_row, doc_id) VALUES ("+global_id_str+", '"+QDate::currentDate().toString("dd.MM.yyyy")+"', "+global_staff_id+", 'false', "+id_document+") RETURNING id");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {
            while(query.next())
            {
                open_new_document(query.value(0).toString());
            }
        }
        load_documents();
    }
    else
    {
        qDebug()<<"server lost";
    }
}
void Dialog_patient::delete_document()
{
    int row = ui->tableView_documents_list->currentIndex().row();
    QString id = ui->tableView_documents_list->model()->index(row,0).data(Qt::DisplayRole).toString();
    QSqlDatabase db = QSqlDatabase::database();
    if(db.open())
    {
        QSqlQuery query;
        query.exec("UPDATE test.text_info_patients SET date_creat = '"+QDate::currentDate().toString("dd.MM.yyyy")+"', staff_add_id = "+global_staff_id+", delete_row = 'true' WHERE text_info_patients.id ="+id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        load_documents();
    }
    else
    {
        qDebug()<<"server lost";
    }
}








QString const Dialog_patient::validate_date(QString date)
{
    QStringList day_list = date.split(".");
    QString day;
    QString month;
    QString year;
    QDate date_valid;
    QString date_valid_string;
    if(day_list.count()==3)
    {
        day = day_list[0];
        month = day_list[1];
        year = day_list[2];
        if(date_valid.setDate(year.toInt(),month.toInt(),day.toInt()))
        {
            date_valid_string = date_valid.toString("dd.MM.yyyy");
            return date_valid_string;
        }
        else
        {
            QMessageBox::information(this,"Не правильная дата","Не правильная дата");
            return "exit";
        }
    }
    else
    {
        QMessageBox::information(this,"Не правильный формат даты","Не правильный формат даты");
        return "exit";

    }
}
void Dialog_patient::keyPressEvent(QKeyEvent *e)
{

    if(e->key()==Qt::Key_Return)
        {

            Dialog_patient DP;
            this->focusNextChild();
        }
    if(e->key()==Qt::Key_Enter)
        {

            Dialog_patient DP;
            this->focusNextChild();
        }
}

void Dialog_patient::point_add_birtday(QString text)
{


        if(!global_param)
        {
    //        QFont font_lineedit;
    //        font_lineedit.setPointSize(11);
    //        ui->lineEdit_house->setFont(font_lineedit);
            ui->lineEdit_date_birtday->setStyleSheet("");
    //        ui->label_house_status->setPixmap(ok_icon.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //        ui->lineEdit_house->setFrame(true);
        }


    switch (text.count()) {
    case 2:
        if(text.lastIndexOf(".")!=1)
        {
        ui->lineEdit_date_birtday->setText(text+".");
        }
        break;
    case 5:

        if(text.lastIndexOf(".")!=4)
        {
            ui->lineEdit_date_birtday->setText(text+".");
        }
            break;
    case 11:
        text.replace(10,1,"");

        ui->lineEdit_date_birtday->setText(text);
        break;

    }
}

void Dialog_patient::point_add_ds_start(QString text)
{
    switch (text.count()) {
    case 2:
        if(text.lastIndexOf(".")!=1)
        {
        ui->lineEdit_date_start->setText(text+".");
        }
        break;
    case 5:

        if(text.lastIndexOf(".")!=4)
        {
            ui->lineEdit_date_start->setText(text+".");
        }
            break;
    case 11:
        text.replace(10,1,"");

        ui->lineEdit_date_start->setText(text);
        break;

    }
}
void Dialog_patient::point_add_ds_stop(QString text)
{
    switch (text.count()) {
    case 2:
        if(text.lastIndexOf(".")!=1)
        {
        ui->lineEdit_date_end->setText(text+".");
        }
        break;
    case 5:

        if(text.lastIndexOf(".")!=4)
        {
            ui->lineEdit_date_end->setText(text+".");
        }
            break;
    case 11:
        text.replace(10,1,"");

        ui->lineEdit_date_end->setText(text);
        break;

    }
}


void Dialog_patient::set_default_color_lineedit_fname()
{
    QPixmap ok_icon(":/icon/png/images/ok.png");
    if(!global_param)
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_fname->setFont(font_lineedit);
        ui->lineEdit_fname->setStyleSheet("border: 2px solid green");
        ui->label_fname_icon->setPixmap(ok_icon.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

}
void Dialog_patient::set_default_color_lineedit_name()
{
    QPixmap ok_icon(":/icon/png/images/ok.png");
    if(!global_param)
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_name->setFont(font_lineedit);
        ui->lineEdit_name->setStyleSheet("border: 2px solid green");
        ui->label_name_icon->setPixmap(ok_icon.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}
void Dialog_patient::set_default_color_lineedit_mname()
{
    QPixmap ok_icon(":/icon/png/images/ok.png");
    if(!global_param)
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_oname->setFont(font_lineedit);
        ui->lineEdit_oname->setStyleSheet("border: 2px solid green");
        ui->label_mname_icon->setPixmap(ok_icon.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}
void Dialog_patient::set_default_color_lineedit_serial_passport()
{
    QFont font_lineedit;
    if(!global_param)
    {
    font_lineedit.setPointSize(11);
    ui->lineEdit_serial_passport->setFont(font_lineedit);
    ui->lineEdit_serial_passport->setStyleSheet("border: 2px solid green");
    ui->lineEdit_serial_passport->setFrame(true);
    }
}
void Dialog_patient::set_default_color_lineedit_number_passport()
{
    if(!global_param)
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_number_passport->setFont(font_lineedit);
        ui->lineEdit_number_passport->setStyleSheet("border: 2px solid green");
        ui->lineEdit_number_passport->setFrame(true);
    }
}

void Dialog_patient::set_default_color_combox_area()
{
    if(!global_param)
    {

        ui->comboBox_area->setStyleSheet("");

    }
}
void Dialog_patient::set_default_color_combox_area_street()
{
    if(!global_param)
    {

        ui->comboBox_area_street->setStyleSheet("");

    }
}
void Dialog_patient::set_default_color_combox_sex()
{
    if(!global_param)
    {

        ui->comboBox_sex->setStyleSheet("");

    }
}

void Dialog_patient::put_all_settings()
{
    qDebug()<<"put_all_settings";

    QSqlDatabase db = QSqlDatabase::database();
    ui->comboBox_sex->clear();
    ui->comboBox_street->clear();
    //ui->tableWidget_text_1->setColumnCount(2);
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
        ui->comboBox_sex->addItem("Ж","false");
        ui->comboBox_sex->addItem("М","true");
        ui->comboBox_area_street->addItem("Выборгский район", "1");
        ui->comboBox_area_street->addItem("Калининский район", "2");
        ui->comboBox_invalid->addItem("",3);
        ui->comboBox_invalid->addItem("I",0);
        ui->comboBox_invalid->addItem("II",1);
        ui->comboBox_invalid->addItem("III",2);
        ui->lineEdit_date_start->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        ui->lineEdit_date_end->setText(QDate::currentDate().toString("dd.MM.yyyy"));
        ui->lineEdit_date_end->hide();
        ui->comboBox_why_close->hide();
        ui->checkBox_ds_end_state->isChecked();


    }
    if(db.open())
    {
        qDebug()<<"load why removed";
        QSqlQuery query;
        query.exec("SELECT why_removed.id, why_removed.description FROM test.why_removed order by id;");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {
            QString id_status = query.value(0).toString();
            QString description = query.value(1).toString();

            ui->comboBox_why_close->addItem(description,id_status);
        }
        query.clear();
    }
    if(db.open())
    {
        qDebug()<<"load area";
        QSqlQuery query;
        query.exec("SELECT id, name FROM test.area");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
            QString id_status = query.value(0).toString();
            QString description = query.value(1).toString();
            ui->comboBox_area->addItem(description,id_status);
        }
        query.clear();
    }
    if(db.open())
    {
        qDebug()<<"load direct";
        QSqlQuery query;
        query.exec("SELECT id, description FROM test.direct");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
            QString id_status = query.value(0).toString();
            QString description = query.value(1).toString();
            ui->comboBox_direction->addItem(description,id_status);
        }
        query.clear();
    }
    if(db.open())
    {
        qDebug()<<"load direct from";
        QSqlQuery query;
        query.exec("SELECT id, description FROM test.direct_from");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
                    QString id_status = query.value(0).toString();
                    QString description = query.value(1).toString();
                    ui->comboBox_direct_from->addItem(description,id_status);
                }
        query.clear();
    }
    if(db.open())
    {
        qDebug()<<"load end table";
        QSqlQuery query;
        query.exec("SELECT id, description FROM test.end");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
                    QString id_status = query.value(0).toString();
                    QString description = query.value(1).toString();
                    ui->comboBox_end->addItem(description,id_status);
                }
        query.clear();
    }
    if(db.open())
    {
        qDebug()<<"load list documents";
        QSqlQuery query;
        query.exec("SELECT id, description FROM test.document WHERE id_end = '1'");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next()) {
                    QString id_status = query.value(0).toString();
                    QString description = query.value(1).toString();
                    ui->comboBox_document->addItem(description,id_status);
                }
                ui->comboBox_document->setCurrentIndex(ui->comboBox_document->findData("1"));
    }
    load_documents();

}

void Dialog_patient::check_data()
{

}
void Dialog_patient::change_state_ds_end(bool state) //событие на изменение галкой состояния
{
    if(state)
    {
        ui->lineEdit_date_end->show();
        ui->lineEdit_date_start->hide();
        ui->label_10->hide();
        ui->label_11->show();
        ui->comboBox_direction->hide();
        ui->comboBox_why_close->show();
        ui->label_direct_from->hide();
        ui->comboBox_direct_from->hide();
    }
    else
    {
        ui->lineEdit_date_end->hide();
        ui->lineEdit_date_start->show();
        ui->label_10->show();
        ui->label_11->hide();
        ui->comboBox_direction->show();
        ui->label_direct_from->show();
        ui->comboBox_direct_from->show();
        ui->comboBox_why_close->hide();
    }
}

void Dialog_patient::setParam(int param, int id, bool show_uchet)
{
    Objects_app obj;
    global_param = param;
    global_staff_id = obj.staff_id;
    global_id_str.setNum(id);
    QIcon icon_add(":/icon/png/images/add_icon.png");
    QIcon icon_edit(":/icon/png/images/edit_icon.png");
    QString id_str;
    id_str.setNum(id);
    switch(param)
    {
    case 0:
        ui->tab_2->setEnabled(false);
        Dialog_patient::setWindowIcon(icon_add);
        Dialog_patient::setWindowTitle("Добавление нового пациента");
        ui->tableView->hide();
        ui->pushButton_add_history->hide();
        ui->comboBox_area->setStyleSheet("border: 3px solid orange");
        ui->comboBox_area_street->setStyleSheet("border: 3px solid orange");
        ui->lineEdit_date_birtday->setStyleSheet("border: 3px solid orange");
        ui->lineEdit_name->setStyleSheet("border: 3px solid orange");
        ui->lineEdit_fname->setStyleSheet("border: 3px solid orange");
        ui->lineEdit_number_passport->setStyleSheet("border: 3px solid orange");
        ui->lineEdit_serial_passport->setStyleSheet("border: 3px solid orange");
        ui->comboBox_sex->setStyleSheet("border: 3px solid orange");
        ui->dateTimeEdit->hide();
        ui->label_10->hide();
        ui->comboBox_direction->hide();
        ui->label_6->hide();
        ui->lineEdit_date_start->hide();
        ui->checkBox_ds_end_state->hide();
        ui->label_9->hide();
        ui->label_direct_from->hide();
        ui->comboBox_direct_from->hide();
        ui->label_11->hide();
        ui->pushButton_edit_date_create_medcard->hide();

        break;
    case 1:
        ui->tab_2->setEnabled(true);
        ui->dateTimeEdit->show();
        ui->dateTimeEdit->setEnabled(true);
        Dialog_patient::setWindowIcon(icon_edit);
        Dialog_patient::setWindowTitle("Изменение информации пациента в медкарте №"+id_str);
        text_change = false;
        load_comments();
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("11"));
        ui->comboBox_document->setCurrentIndex(ui->comboBox_document->findData("31"));
        load_documents();
        get_data_sql(id);
        break;
    case 2:
        ui->lineEdit_fname->setReadOnly(true);
        ui->lineEdit_name->setReadOnly(true);
        ui->lineEdit_oname->setReadOnly(true);
        ui->lineEdit_date_birtday->setReadOnly(true);
        ui->lineEdit_date_end->setReadOnly(true);
        ui->lineEdit_date_start->setReadOnly(true);
        ui->lineEdit_work_or_education->setReadOnly(true);
        ui->comboBox_street->setEditable(false);
        ui->lineEdit_house->setReadOnly(true);
        ui->lineEdit_korpuse->setReadOnly(true);
        ui->lineEdit_room->setReadOnly(true);
        ui->lineEdit_telefon->setReadOnly(true);
        ui->lineEdit_index->setReadOnly(true);
        ui->lineEdit_serial_passport->setReadOnly(true);
        ui->lineEdit_number_passport->setReadOnly(true);
        ui->lineEdit_polis_number->setReadOnly(true);
        ui->lineEdit_polis_serial->setReadOnly(true);
        ui->lineEdit_snils->setReadOnly(true);
        ui->lineEdit_group_lgot->setReadOnly(true);
        ui->lineEdit_passport_vidan->setReadOnly(true);
        ui->checkBox_pt->setEnabled(false);
        ui->checkBox_ds_end_state->setEnabled(false);
        ui->checkBox_custody->setEnabled(false);
        ui->pushButton_document_create->setEnabled(false);
        ui->pushButton_document_delete->setEnabled(false);
        ui->pushButton_document_edit->setEnabled(false);

        ui->pushButton_add_history->hide();
        ui->pushButton_apply->hide();
        ui->pushButton_ok->hide();
        ui->pushButton_edit_date_create_medcard->hide();
        ui->tab_2->setEnabled(true);
        load_comments();
        if(!show_uchet)
        {
            ui->tableView->hide();
        }
        else
        {
            ui->tableView->show();
        }
        Dialog_patient::setWindowIcon(icon_edit);
        Dialog_patient::setWindowTitle("Просмотр информации пациента в медкарте №"+id_str);
        text_change = false;
        get_data_sql(id);
        ui->pushButton_cancel->setText("Закрыть");
        break;
    case 3:
        ui->tab_2->setEnabled(true);
        ui->dateTimeEdit->show();
        ui->dateTimeEdit->setEnabled(true);
        Dialog_patient::setWindowIcon(icon_edit);
        Dialog_patient::setWindowTitle("Изменение информации пациента в медкарте №"+id_str);
        text_change = false;
        load_comments();
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("11"));
        ui->comboBox_document->setCurrentIndex(ui->comboBox_document->findData("31"));
        load_documents();
        get_data_sql(id);
        ui->tabWidget->setCurrentIndex(1);
        break;
    }

}

void Dialog_patient::get_data_sql(int id)
{
    qDebug()<<"start load patient";
    QSqlDatabase db = QSqlDatabase::database();
    global_id = id;
    if(db.open())
    {
        QSqlQuery query;
        QString id_str;
        id_str.setNum(id);
        qDebug()<<"patient "+id_str;
        query.exec("SELECT patient.fname, patient.name, patient.mname, medcard.sex,  patient.serial_passport, patient.number_passport, medcard.birthday, medcard.job_place, medcard.tutor, medcard.pt, medcard.group_lgot_preparat, medcard.area_id, medcard.serial_snils, medcard.district_id, medcard.group_inv_obsh, medcard.polis_serial, medcard.polis_number, patient.vidan, medcard.time_stamp  FROM test.patient, test.medcard  WHERE medcard.patient_id = patient.id AND medcard.id ="+id_str);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while (query.next())
        {
            QString fname_value = query.value(0).toString().replace(" ","");
            QString name_value = query.value(1).toString().replace(" ","");
            QString mname_value = query.value(2).toString().replace(" ","");
            QString sex_value = query.value(3).toString();
            QString serial_passport = query.value(4).toString();
            QString number_passport = query.value(5).toString();
            QString date_birthday = query.value(6).toDate().toString("dd.MM.yyyy");
            QString job_place_value = query.value(7).toString();
            bool tutor_value = query.value(8).toBool();
            bool pt_value = query.value(9).toBool();
            QString group_lgot_preparat_value = query.value(10).toString();
            QString area_id_value = query.value(11).toString();
            QString snils_serial_value = query.value(12).toString();
            QString district_value = query.value(13).toString();
            QString invalid = query.value(14).toString();
            QString polis_serial_value = query.value(15).toString();
            QString polis_number_value = query.value(16).toString();
            QString passport_vidan = query.value(17).toString();
            QDateTime date_create_medcard = query.value(18).toDateTime();


            ui->lineEdit_fname->setText(fname_value);
            ui->lineEdit_name->setText(name_value);
            ui->lineEdit_oname->setText(mname_value);
            ui->comboBox_sex->setCurrentIndex(ui->comboBox_sex->findData(sex_value));
            ui->lineEdit_serial_passport->setText(serial_passport);
            ui->lineEdit_number_passport->setText(number_passport);
            ui->lineEdit_date_birtday->setText(date_birthday);
            ui->lineEdit_work_or_education->setText(job_place_value);
            ui->checkBox_custody->setChecked(tutor_value);
            ui->checkBox_pt->setChecked(pt_value);
            ui->lineEdit_group_lgot->setText(group_lgot_preparat_value);
            ui->comboBox_area->setCurrentIndex(ui->comboBox_area->findData(area_id_value));
            ui->lineEdit_polis_number->setText(polis_number_value);
            ui->lineEdit_polis_serial->setText(polis_serial_value);
            ui->comboBox_invalid->setCurrentIndex(ui->comboBox_invalid->findData(invalid));
            ui->comboBox_area_street->setCurrentIndex(ui->comboBox_area_street->findData(district_value));
            ui->lineEdit_snils->setText(snils_serial_value);
            ui->lineEdit_passport_vidan->setText(passport_vidan);
            ui->dateTimeEdit->setDateTime(date_create_medcard);
        }
        query.clear();
#ifdef _WIN32
  Sleep(200);
#endif
        if(db.open())
        {
            query.exec("SELECT address_patient.street_id, address_patient.building, address_patient.home, address_patient.flat, address_patient.telefon, address_patient.index_adrr FROM test.address_patient WHERE medcard_id ="+id_str);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while (query.next())
            {
                QString street_value = query.value(0).toString();
                QString building_value = query.value(1).toString();
                QString house_value = query.value(2).toString();
                QString flat = query.value(3).toString();
                QString telefon_value = query.value(4).toString();
                QString index_street_value = query.value(5).toString();

                ui->comboBox_street->setCurrentIndex(ui->comboBox_street->findData(street_value));
                ui->lineEdit_korpuse->setText(building_value);
                ui->lineEdit_house->setText(house_value);
                ui->lineEdit_telefon->setText(telefon_value);
                ui->lineEdit_room->setText(flat);
                ui->lineEdit_index->setText(index_street_value);
            }
        }

 }

table_uchet_history();
}
void Dialog_patient::table_uchet_history()
{
    qDebug()<<"load_table uchet";
    QSqlDatabase db = QSqlDatabase::database();

    model_uchet = new QSqlRelationalTableModel();
    if(db.open())
    {
    model_uchet->setTable("test.uchet");
    model_uchet->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_uchet->setRelation(4, QSqlRelation("test.why_removed", "id", "description"));
    model_uchet->setRelation(6, QSqlRelation("test.direct", "id", "description"));
    model_uchet->setRelation(7, QSqlRelation("test.direct_from", "id", "description"));
    model_uchet->setFilter("uchet.medcard_id = "+global_id_str+"AND uchet.delete_row = false");
    model_uchet->sort(0,Qt::SortOrder());
    model_uchet->setHeaderData(2, Qt::Horizontal, tr("Дата\nобращения"));
    model_uchet->setHeaderData(3, Qt::Horizontal, tr("Дата\nархивирования"));
    model_uchet->setHeaderData(4, Qt::Horizontal, tr("Причина\nархивирования"));
    model_uchet->setHeaderData(6, Qt::Horizontal, tr("Направлен"));
    model_uchet->setHeaderData(7, Qt::Horizontal, tr("Откуда"));

    model_uchet->select();
    if(model_uchet->lastError().isValid())
    {
        qDebug()<<model_uchet->lastError();
    }
    qDebug()<<"table_load_ok";
    }
    else
    {
     qDebug()<<"lost server";
    }
    qDebug()<<"add model to table";
    ui->tableView->setModel(model_uchet);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(5);
    ui->tableView->hideColumn(8);
    ui->tableView->hideColumn(9);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qDebug()<<"model load good";

}

void Dialog_patient::apply_send_data_sql()
{
    qDebug()<<"apply send data sql";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString id_str;
    Objects_app obj;
    id_str.setNum(global_id);
    QString fname_value = ui->lineEdit_fname->text();
    QString name_value = ui->lineEdit_name->text();
    QString oname_value = ui->lineEdit_oname->text();
    QString serial_passport = ui->lineEdit_serial_passport->text();
    QString number_passport = ui->lineEdit_number_passport->text();
    QString sex_value = ui->comboBox_sex->currentData().toString();
    QString home = ui->lineEdit_house->text();
    QString building = ui->lineEdit_korpuse->text();
    QString flat = ui->lineEdit_room->text();
    QString street_id = ui->comboBox_street->currentData().toString();

    QString date_birthday;

    QString date_birthday_test = validate_date(ui->lineEdit_date_birtday->text());
    if(date_birthday_test=="exit")
    {
        return;
    }
    else
    {
        date_birthday=date_birthday_test;
    }



    QString telefon_value = ui->lineEdit_telefon->text();
    QString id_patient;
    QString id_medcard;
    QString job_place_value = ui->lineEdit_work_or_education->text();
    QString area_value = ui->comboBox_area->currentData().toString();
    QString tutor_value;
    QString pt_value;
    QString group_lgot_value = ui->lineEdit_group_lgot->text();
    QString polis_serial_value = ui->lineEdit_polis_serial->text();
    QString polis_number_value = ui->lineEdit_polis_number->text();
    QString snils_serial_value = ui->lineEdit_snils->text();
    QString district_value = ui->comboBox_area_street->currentData().toString();
    QString direct_value = ui->comboBox_direction->currentData().toString();
    QString direct_from_value = ui->comboBox_direct_from->currentData().toString();
    QString group_inv_value = ui->comboBox_invalid->currentData().toString();
    QString index_street = ui->lineEdit_index->text();
    QString ds_end;
    QString ds_start;
    QString ds_start_test = validate_date(ui->lineEdit_date_start->text());
    QString why_remove;
    QString passport_vidan_value = ui->lineEdit_passport_vidan->text();

    if(ui->checkBox_ds_end_state->checkState()==Qt::Checked)
    {
        QString ds_end_test = validate_date(ui->lineEdit_date_end->text());
        if(ds_end_test=="exit")
        {
            return;
        }
        else
        {
            ds_end=ds_end_test;
        }
        why_remove = ui->comboBox_why_close->currentData().toString();

    }
    else if(ui->checkBox_ds_end_state->checkState()==Qt::Unchecked)
    {
        ds_end = "";
        why_remove="-1";
    }


    if(ds_start_test=="exit")
    {
        return;
    }
    else
    {
        ds_start=ds_start_test;
    }

    if(ui->checkBox_custody->checkState()==Qt::Checked)
    {
        tutor_value = "1";

    }
    else if(ui->checkBox_custody->checkState()==Qt::Unchecked)
    {
        tutor_value = "0";
    }
    if(ui->checkBox_pt->checkState()==Qt::Checked)
    {
        pt_value = "1";

    }
    else if(ui->checkBox_pt->checkState()==Qt::Unchecked)
    {
        pt_value = "0";
    }

    int control_lineedit=0;
    QPixmap error_pix(":/icon/png/images/Warning.png");

    if(ui->lineEdit_fname->text()=="")
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_fname->setFont(font_lineedit);
        ui->lineEdit_fname->setStyleSheet("border: 2px solid red");
        ui->label_fname_icon->setPixmap(error_pix.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        control_lineedit=1;
    }
    else
    {
        QPalette p = ui->lineEdit_fname->palette();
        p.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_fname->setPalette(p);
        ui->lineEdit_fname->setFrame(true);
    }
    if(ui->lineEdit_name->text()=="")
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_name->setFont(font_lineedit);
        ui->lineEdit_name->setStyleSheet("border: 2px solid red");
        ui->label_name_icon->setPixmap(error_pix.scaled(24,24,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        control_lineedit=1;
    }
    else
    {
        QPalette p = ui->lineEdit_name->palette();
        p.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_name->setPalette(p);
        ui->lineEdit_name->setFrame(true);
    }

    if(ui->lineEdit_serial_passport->text()=="")
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_serial_passport->setFont(font_lineedit);
        ui->lineEdit_serial_passport->setStyleSheet("border: 2px solid red");
        control_lineedit=1;
    }
    else
    {
        QPalette p = ui->lineEdit_serial_passport->palette();
        p.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_serial_passport->setPalette(p);
        ui->lineEdit_serial_passport->setFrame(true);
    }
    if(ui->lineEdit_number_passport->text()=="")
    {
        QFont font_lineedit;
        font_lineedit.setPointSize(11);
        ui->lineEdit_number_passport->setFont(font_lineedit);
        ui->lineEdit_number_passport->setStyleSheet("border: 2px solid red");
        control_lineedit=1;
    }
    else
    {
        QPalette p = ui->lineEdit_number_passport->palette();
        p.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_number_passport->setPalette(p);
        ui->lineEdit_number_passport->setFrame(true);
    }
    if(control_lineedit==0)
    {
        if(db.open())
        {
            switch (global_param) {
            case 0: //insert
                if(ui->checkBox_ds_end_state->checkState()==Qt::Unchecked)
                {
                    query.exec("SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', 99, "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', '"+ds_start+"', NULL, '"+why_remove+"', "+direct_value+", "+direct_from_value+")");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                else if(ui->checkBox_ds_end_state->checkState()==Qt::Checked)
                {
                    query.exec("SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', "+why_remove+", "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', NULL, '"+ds_end+"', '"+why_remove+"', -1, -1)");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                ui->lineEdit_fname->setText("");
                ui->lineEdit_name->setText("");
                ui->lineEdit_oname->setText("");
                ui->lineEdit_serial_passport->setText("");
                ui->lineEdit_number_passport->setText("");
                ui->lineEdit_house->setText("");
                ui->lineEdit_korpuse->setText("");
                ui->lineEdit_room->setText("");
                Dialog_patient::accept();
                break;
            case 1: //update


                if(db.open())
                {
                    query.exec("UPDATE test.medcard SET area_id = '"+area_value+"', sex = "+sex_value+", birthday='"+date_birthday+"', job_place='"+job_place_value+"', tutor='"+tutor_value+"', pt='"+pt_value+"', group_lgot_preparat='"+group_lgot_value+"', serial_snils='"+snils_serial_value+"', polis_number='"+polis_number_value+"', polis_serial = '"+polis_serial_value+"', district_id="+district_value+", group_inv_obsh='"+group_inv_value+"' WHERE id ="+id_str);
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                    while (query.next())
                    {
                        id_patient = query.value(0).toString();
                    }
                }
                else
                {
                    qDebug()<<"server lost "<<"UPDATE test.medcard";
                }

                if(id_patient=="")
                {
                    if(db.open())
                    {
                        query.exec("SELECT medcard.patient_id FROM test.medcard WHERE medcard.id="+id_str);
                        if(query.lastError().isValid())
                        {
                            qDebug()<<query.lastError();
                            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                        }
                        while (query.next())
                        {
                            id_patient = query.value(0).toString();
                        }
                    }
                    else
                    {
                        qDebug()<<"server lost "<<"SELECT medcard.patient_id";
                    }
                }

                if(db.open())
                {
                    query.exec("UPDATE test.patient SET fname ='"+fname_value+"', name ='"+name_value+"', mname = '"+oname_value+"',serial_passport = '"+serial_passport+"', number_passport = '"+number_passport+"', vidan='"+passport_vidan_value+"' WHERE id = "+id_patient);
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                else
                {
                    qDebug()<<"server lost "<<"UPDATE test.patient";
                }
#ifdef _WIN32
  Sleep(100);
#endif
                if(db.open())
                {
                    query.exec("UPDATE test.address_patient SET street_id ='"+street_id+"', home = '"+home+"', building = '"+building+"', flat = '"+flat+"', telefon='"+telefon_value+"', index_adrr = '"+index_street+"' WHERE medcard_id = "+id_str);
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                else
                {
                    qDebug()<<"server lost "<<"UPDATE test.address_patient";
                }
#ifdef _WIN32
  Sleep(100);
#endif
                if(db.open())
                {
                    query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Обновлена медкарта"+id_str+"')");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                else
                {
                    qDebug()<<"server lost "<<"INSERT INTO test.logs";
                }
                Dialog_patient::accept();
                break;
            }
        }
    }

}
void Dialog_patient::ok_send_data_sql()
{
    apply_send_data_sql();

    Dialog_patient::accept();

}
void Dialog_patient::button_cancel()
{
    if(global_param == 1)
    {
    //save_text_document();
    }
    Dialog_patient::close();
}
void Dialog_patient::add_street()
{
    qDebug()<<"add street";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QString add_street_text = ui->comboBox_street->currentText();
    if(db.open())
    {
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', '"+obj.staff_name+": Требуется добавить улицу: "+add_street_text+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
#ifdef _WIN32
  Sleep(100);
#endif
        //query.exec("INSERT INTO test.street(name, staff_add_id) VALUES ('"+add_street_text+"', '"+obj.staff_id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        QMessageBox::information(this,"Добавление улицы","Отправлен запрос на добавление улицы :'"+add_street_text+"'");
    }
    put_all_settings();
}
void Dialog_patient::add_histry() // кнопка добавить историю
{
    qDebug()<<"add history uchet";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QString ds_end;
    QString ds_start;
    QString ds_start_test = validate_date(ui->lineEdit_date_start->text());
    QString why_remove;
    QString direct_value = ui->comboBox_direction->currentData().toString();
    QString direct_from_value = ui->comboBox_direct_from->currentData().toString();
    QString patient_id;
    if(ui->checkBox_ds_end_state->isChecked() && ui->comboBox_why_close->currentData().toString() == "-1")
    {
        QMessageBox::information(this,"Ошибка","Невыбрана причина закрытия!");
        return;
    }
    if(db.open())
    {
        query.exec("SELECT medcard.patient_id FROM test.medcard WHERE medcard.id = "+global_id_str);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }

        while(query.next())
        {
            patient_id = query.value(0).toString();
            qDebug()<<patient_id;
        }
        qDebug()<<patient_id;
    }
    if(ds_start_test=="exit")
    {
        return;
    }
    else
    {
        ds_start=ds_start_test;
    }

    if(ui->checkBox_ds_end_state->checkState()==Qt::Checked)
    {
        QString ds_end_test = validate_date(ui->lineEdit_date_end->text());
        if(ds_end_test=="exit")
        {
            return;
        }
        else
        {
            ds_end=ds_end_test;
            why_remove = ui->comboBox_why_close->currentData().toString();
            query.exec("INSERT INTO test.uchet(staff_add_id, on_uchet, off_uchet, why_removed_id, medcard_id, direct, direct_from) VALUES ('"+obj.staff_id+"', NULL, '"+ds_end+"', '"+why_remove+"', '"+global_id_str+"', '99', '-1');");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            if(db.open())
            {
                query.exec("UPDATE test.dynamic_view SET status='true', cause='В Архив', staff_add_id = '"+obj.staff_id+"',stop_date = '"+QDate::currentDate().toString("dd.MM.yyyy")+"' WHERE medcard_id = "+global_id_str);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            if(db.open())
            {
                query.exec("UPDATE test.patient SET status="+why_remove+" WHERE id = "+patient_id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }



            else
            {
                qDebug()<<"server lost";
            }
        }
    }
    else if(ui->checkBox_ds_end_state->checkState()==Qt::Unchecked)
    {
        ds_end = "";
        why_remove="-1";
        query.exec("INSERT INTO test.uchet(staff_add_id, on_uchet, off_uchet, why_removed_id, medcard_id, direct, direct_from ) VALUES ('"+obj.staff_id+"', '"+ds_start+"', NULL, '-1', '"+global_id_str+"', '"+direct_value+"', '"+direct_from_value+"');");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        if(db.open())
        {
            query.exec("UPDATE test.patient SET status="+direct_value+" WHERE id = "+patient_id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        else
        {
            qDebug()<<"server lost";
        }
    }
    table_uchet_history();
}
void Dialog_patient::edit_date_create_medcard()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
    QString dateTime = ui->dateTimeEdit->dateTime().toString("dd.MM.yyyy HH:mm");
    query.exec("UPDATE test.medcard SET time_stamp='"+dateTime+"' WHERE id ="+global_id_str);
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    }
    else
    {
        qDebug()<<"Server lost connection";
    }
}
void Dialog_patient::add_comments()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString comment = ui->textEdit_comment->toPlainText();
    if(param_comments == 1)
    {

        if(db.open())
        {
            query.exec("UPDATE test.comments SET comment = '"+ui->textEdit_comment->toPlainText()+"' WHERE id = "+edit_comments_id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            ui->textEdit_comment->clear();
            param_comments = 0;
        }
    }
    else
    {
        if(db.open())
        {
            query.exec("INSERT INTO test.comments(type, staff, comment, medcard_id) VALUES (0, "+obj.staff_id+",'"+comment+"', "+global_id_str+");");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            ui->textEdit_comment->clear();
        }
    }
    model_comments->select();
}
void Dialog_patient::edit_comments()
{
    int selected_tables = ui->tableView_comments_patient->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_comments_patient->currentIndex().row();
        param_comments = 1;
        edit_comments_id = ui->tableView_comments_patient->model()->index(row,0).data(Qt::DisplayRole).toString();
        ui->textEdit_comment->setText(ui->tableView_comments_patient->model()->index(row,5).data(Qt::DisplayRole).toString());
    }

}
void Dialog_patient::load_comments()
{
    model_comments = new QSqlTableModel();
    QFont font;
    font.setPointSize(13);
    model_comments->setTable("test.comments_view");
    model_comments->setFilter("type = 0 AND medcard_id = "+global_id_str);
    model_comments->setSort(0,Qt::AscendingOrder);
    model_comments->select();


    ui->tableView_comments_patient->setFont(font);
    ui->tableView_comments_patient->setModel(model_comments);
    ui->tableView_comments_patient->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableView_comments_patient->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    ui->tableView_comments_patient->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->tableView_comments_patient->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_comments_patient->hideColumn(0);
    ui->tableView_comments_patient->hideColumn(1);
    ui->tableView_comments_patient->hideColumn(2);
    //ui->tableView_comments_patient->hideColumn(3);
    ui->tableView_comments_patient->hideColumn(4);
    ui->tableView_comments_patient->hideColumn(6);
}
void Dialog_patient::context_menu_comments(QPoint pos)
{
    QString id;
    QString staff;
    Objects_app obj;
    int selected_tables = ui->tableView_comments_patient->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_comments_patient->currentIndex().row();
        staff = ui->tableView_comments_patient->model()->index(row,2).data(Qt::DisplayRole).toString();
    }
    QMenu *menu = new QMenu;
    menu->addAction("Скопировать только текст", this, SLOT(copy_text_comments()));
    if(obj.staff_id == staff)
    {
        menu->addAction("Редактировать", this, SLOT(edit_comments()));
    }
    menu->exec(ui->tableView_comments_patient->mapToGlobal(pos));
}
void Dialog_patient::copy_text_comments()
{

    int selected_tables = ui->tableView_comments_patient->currentIndex().row();
    qDebug()<<"Rows in Table "<< selected_tables;
    if (selected_tables >= 0)
    {
        int row = ui->tableView_comments_patient->currentIndex().row();
        qDebug()<<ui->tableView_comments_patient->model()->index(row,5).data(Qt::DisplayRole).toString();
        QApplication::clipboard()->setText(ui->tableView_comments_patient->model()->index(row,5).data(Qt::DisplayRole).toString());

    }
}
void Dialog_patient::del_uchet()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        QString begin_status;

        int ret = QMessageBox::warning(this, tr("Удаление из таблицы учет"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {
            query.exec("UPDATE test.uchet  SET delete_row='true' WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }

            if(db.open())
            {
                query.exec("SELECT CASE WHEN why_removed_id::int <0 THEN direct WHEN why_removed_id::int>=0 THEN why_removed_id END FROM test.uchet WHERE medcard_id = "+global_id_str+" AND delete_row = false order by id DESC LIMIT 1");
            }
            if(query.size()>0)
            {
            while(query.next())
            {
                begin_status = query.value(0).toString();
            }
            }
            else
            {
                begin_status = "99";
            }
            if(db.open())
            {
                query.exec("UPDATE test.patient  SET status="+begin_status+" WHERE id = (SELECT patient.id FROM test.patient, test.medcard WHERE medcard.patient_id = patient.id AND medcard.id = "+global_id_str+")");
            }
            qDebug()<<begin_status;
            model_uchet->select();
        }

    }
    else
    {
        QMessageBox::warning(this,"Ошибка", "Выберете одного пациента");

    }
}
