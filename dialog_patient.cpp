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
    QString body_doc = NULL;
    QSqlQuery query;

    int ret = QMessageBox::warning(this, tr("Создание по шаблону?"),
                                   tr("Создать документ по шаблону?"),
                                   QMessageBox::Yes|QMessageBox::No);

    if(ret==16384)
    {

        switch (id_document.toInt()) {
        case 0:

            break;
        case 1:
                body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-size:14pt; font-weight:600;\">Протокол заседания ВК №</span><span style=\" font-family:'Calibri'; font-size:14pt;\"> </span><span style=\" font-family:'Calibri'; font-size:14pt; font-style:italic;\">___</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Дата:</span><span style=\" font-family:'Calibri';\"> ___________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Ф.И.О.:  $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Дата рождения:  $BIRTHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Адрес: $ADRESS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Социальный статус</span><span style=\" font-family:'Calibri';\">: ________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Цель представления на ВК:</span><span style=\" font-family:'Calibri'; font-weight:600; text-decoration: underline;\">     </span><span style=\" font-family:'Calibri'; text-decoration: underline;\">предоставление академического отпуска</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Анамнестические сведения (особенности течения  и динамика заболевания):</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\"> </span><span style=\" font-family:'Times New Roman';\">________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Страховой анамнез: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">____________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Объективно:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Жалобы:</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">________________________________________________________________________________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Решение ВК:</span><span style=\" font-family:'Calibri';\">   </span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; text-decoration: underline;\">По состоянию здоровья предоставить академический отпуск</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Председатель ВК:</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-weight:600;\">Члены ВК:</span> </p></td></tr></table></body></html>";
                if(db.open())
                {
                    query.exec("SELECT \
                               all_view_patient.id, \
                               concat(all_view_patient.fname, ' ',\
                               all_view_patient.name, ' ',\
                               all_view_patient.mname), \
                               all_view_patient.birthday, \
                               concat(street.name, \
                                                CASE\
                                                  WHEN address_patient.building ='' THEN ''\
                                                  ELSE ' корп.'\
                                                END , \
                                                address_patient.building, ' д.',\
                                                address_patient.home, ' кв.',\
                                                address_patient.flat) AS Street,\
                               diagnos.code  \
                             FROM \
                               test.all_view_patient, \
                               test.address_patient, \
                               test.street, \
                               test.diagnos_patient, \
                               test.diagnos\
                             WHERE \
                               all_view_patient.id = diagnos_patient.medcard_id AND\
                               address_patient.medcard_id = all_view_patient.id AND\
                               street.id = address_patient.street_id AND\
                               diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                            while(query.next())
                                {
                                    body_doc.replace("$FIO$",query.value(1).toString());
                                    body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                    body_doc.replace("$ADRESS$",query.value(3).toString());
                                    body_doc.replace("$DIAGNOS$",query.value(4).toString());
                                }
                            }
            break;
        case 2:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"0\">\n<tr>\n<td width=\"336\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"data:image/jpeg;base64,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\" width=\"74\" height=\"83\" style=\"vertical-align: top;\" /> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">          ПРАВИТЕЛЬСТВО санкт-петербурга                        </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">                            АДМИНИСТРАЦИЯ                               </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">Выборгского района <br />Санкт-Петербурга</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Санкт-Петербургское </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">государственное бюджетное учреждение здравоохранения</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">«Психоневрологический диспансер №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">СПб ГБУЗ «ПНД №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Фермское шоссе, д. 34, Санкт-Петербург, 197341</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Тел./Факс (812) 302-11-01</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">E-mail: </span><a href=\"mailto:pnd2@zdrav.spb.ru\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-decoration: underline; color:#0000ff;\">pnd2@zdrav.spb.ru</span></a><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">                    ИНН/КПП 7802091710/781401001</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          От __\u00AD\u00AD\u00AD\u00AD___________     №______________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          На №____________     от   ______________</span><span style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; text-decoration: underline;\">               </span></p></td>\n<td width=\"319\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Куда:_______________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Кому:_______________________  </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p></td></tr></table>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:16pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">       </span>                                    </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">СПРАВКА</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">для предоставления академического отпуска</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; text-decoration: underline;\">   $FIO$  $BIRTHDAY$</span><span style=\" font-family:'Times New Roman';\">, зарегистрированный(ая) по адресу:</span><span style=\" font-family:'Calibri'; text-decoration: underline;\">   $ADRESS$__</span><span style=\" font-family:'Times New Roman';\">, находился(ась) на студенческой справке: _________________________________________________________________</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman';\">(______ дня(ей)).</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Состояние больного(ой) остаётся неустойчивым, несмотря на проведенную терапию.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">Заключение ВК:</span><span style=\" font-family:'Times New Roman';\"> учитывая необходимость продолжения активного медикаментозного лечения, целесообразно предоставление академического отпуска.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Председатель ВК: </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                     Члены ВК:                                                                                                                                                  </span></p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());

                            }
                        }
            break;
        case 3:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">00.00.20__                 Эпикриз</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman'; font-weight:600;\">на ВК для предоставления академического отпуска</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Ф.И.О.: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">__$FIO$_________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Адрес</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">:____$ADRESS$</span><span style=\" font-family:'Times New Roman';\">________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Наблюдался(ась) врачом-психиатром  с ________года с диагнозом:  </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">___</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Инвалидность имеет/не имеет.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечился(ась) в ПБ  </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">_(сколько раз)_.</span><span style=\" font-family:'Times New Roman';\"> Последняя выписка из ПБ  в _________году</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Получает поддерживающее лечение:  </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">_(препараты)_______________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Анамнез: (</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">подробный в динамике)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Психический статус:  (</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">информативный)</span><span style=\" font-family:'Times New Roman';\">.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Страховой анамнез: является студентом(кой) ____ курса  (</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">название учебного заведения)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">За   _______ год справка студенту(ке) выдавалась: (</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">сколько раз)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">с ___________ по ____________№ _____  в  количестве  ___</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">(количество  дней)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">с ___________ по ____________№ _____  в  количестве  ____</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">(количество  дней)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Общее число дней нетрудоспособности:</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\"> ______ (количество дней) </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Диагноз</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">:__(из программы последний диагноз)</span><span style=\" font-family:'Times New Roman';\">_________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> Трудоспособен / Нетрудоспособен.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">На основании освидетельствования пациента(ки) и/или медицинской документации представить на ВК для предоставления академического отпуска.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Заведующий отделением:                                                            /____________________/</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечащий врач:                                                                              /____________________/</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 4:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">                                                                </span><span style=\" font-size:12.5pt; font-weight:600;\">АКТ</span> </p>\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt; font-weight:600;\">психиатрического освидетельствования</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">На продление амбулаторного принудительного лечения <span style=\" text-decoration: underline;\"></span><span style=\" text-decoration: underline;\">$FIO$</span>, зарегистрированного(ой) по адресу: <span style=\" text-decoration: underline;\"></span><span style=\" text-decoration: underline;\">$ADRESS$,</span> которому(ой) было назначено амбулаторное принудительное лечение в СПб ГБУЗ «ПНД №2», согласно приговору ________________________________ от <span style=\" font-family:'Times New Roman';\">_______________г.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Освидетельствование проведено комиссией врачей-психиатров в составе: <span style=\" color:#000000;\">Председатель врачебной комиссии: ____________________, </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">Члены врачебной комиссии: </span><span style=\" font-family:'Times New Roman'; color:#000000;\">____________________________________________________, Лечащий врач:_____________________________.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Обстоятельства дела:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">__________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">             </span><span style=\" font-weight:600; text-decoration: underline;\">Анамнез:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">_______________________________________________________________________________________________________________________________________________________________.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">На момент осмотра:</span> <span style=\" font-family:'Times New Roman';\">_______________________________________________________________________________________________________________________________________________________________. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Заключение:</span> на основании вышеизложенного врачебная комиссия пришла к выводу, что: </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">1.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">                 </span><span style=\" font-family:'Times New Roman';\">_______________________________________________________________.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">2.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">                 </span><span style=\" font-family:'Times New Roman';\">_______________________________________________________________.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">3.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">                 </span><span style=\" font-family:'Times New Roman';\">По своему психическому состоянию может присутствовать на суде.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Председатель врачебной комиссии:                                                  /____________________/ </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Члены врачебной комиссии:                                              <span style=\" font-family:'Times New Roman';\">                 /____________________/</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">                                                                                                                /____________________/ </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечащий врач                                                                                       /____________________/</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 5:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt; font-weight:600;\">ПРЕДСТАВЛЕНИЕ</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">            <span style=\" font-size:12.5pt;\">На продление амбулаторного принудительного лечения (АПЛ) $FIO$, зарегистрированному(ой) по адресу: $ADRESS$</span><span style=\" font-size:12.5pt; text-decoration: underline;\">,</span><span style=\" font-size:12.5pt;\"> которому(ой) согласно приговору ______________________________ от </span><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\"> _____________г. была назначена мера пресечения в виде</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\">амбулаторного принудительного наблюдения и лечения  у психиатра.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt; color:#000000;\">________________г.</span><span style=\" font-size:12.5pt;\"> $FIO$ был(а) освидетельствован(а)  комиссией врачей-психиатров СПб ГБУЗ «ПНД №2», которая пришла к заключению о необходимости продления амбулаторного принудительного лечения.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\">Руководствуясь ст. 102 УК РФ, ходатайствую о продлении амбулаторного принудительного лечения</span><span style=\" font-family:'Times New Roman';\"> $FIO$ </span><span style=\" font-size:12.5pt;\">в СПб ГБУЗ «ПНД №2». </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt; font-weight:600;\">ПРИЛОЖЕНИЕ:</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\">1.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">     </span><span style=\" font-size:12.5pt;\">АКТ психиатрического освидетельствования на </span><span style=\" font-family:'Times New Roman';\">___(</span><span style=\" font-family:'Times New Roman'; font-size:12.5pt; text-decoration: underline;\">Ф.И.О. из программы</span><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\">)___ </span><span style=\" font-family:'Times New Roman'; font-size:12.5pt; color:#000000;\">от __________________________г.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-size:12.5pt;\">2.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">     </span><span style=\" font-size:12.5pt;\">Копия приговора ___________________________ от </span><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\">___________________г.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\">3.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">     </span><span style=\" font-size:12.5pt;\">Копия постановления __________________________ от ________________г.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\">4.</span><span style=\" font-family:'Times New Roman'; font-size:7pt;\">     </span><span style=\" font-size:12.5pt;\">Заявление </span><span style=\" font-family:'Times New Roman';\">___(</span><span style=\" font-family:'Times New Roman'; font-size:12.5pt; text-decoration: underline;\">Ф.И.О. из программы</span><span style=\" font-family:'Times New Roman'; font-size:12.5pt;\">)___ об ознакомлении с актом о продлении принудительного лечения.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12.5pt;\">             Главный врач                                                                       О.А. Караулов</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 6:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600; text-decoration: underline; color:#000000;\">Дата</span><span style=\" font-weight:600;\">   </span>                          <span style=\" font-family:'Times New Roman'; font-weight:600;\">Эпикриз на ВК</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                      для решения вопроса о продлении АПЛ/АДН (взятии на АДН)</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; color:#000000;\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">$FIO$</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Совершил(а) общественно-опасное деяние от _____________г. по ст. __________________ УК РФ (текст статьи).</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">_____________________________ суд приговором от ___________ года назначил меру пресечения в отношении $FIO$ $BIRTHDAY$ в виде амбулаторного принудительного наблюдения и лечения  у врача-психиатра.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дата постановки на АПЛ в СПб ГБУЗ &quot;ПНД №2&quot; ______________________г.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Постановлением ____________________________ суда от _____________________г. амбулаторное принудительное лечение продлевалось.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Анамнез: ________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">За истекший период состояние ______________________. Пациент(ка) приходил(а) ежемесячно на прием в СПб ГБУЗ «ПНД №2», рекомендуемую терапию _________________________________. Работает ___________________. Психическое состояние остается неустойчивым.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">         Заведующий отделением                                                      /____________________/</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">       </span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">         Лечащий врач                                                                         /____________________/</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 8:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Дата:_________                     Эпикриз</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman'; font-weight:600;\">на ВК</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                 на изменения диагноза</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Наблюдается с ______ года  с диагнозом: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">____________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Инвалидность: инвалид / не инвалид</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Анамнез:___________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечился(ась) в психиатрических стационарах (даты), последняя выписка (от какого числа, с каким диагнозом): _____________________________________________________________________________  Получает поддерживающее лечение: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">____(препараты)_____.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Клиническая картина: __________________________________________________________________________________________________________________________________________________________. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Психический статус: __________________________________________________________________________________________________________________________________________________________.Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">Исходя из данных анамнеза и клинической картины, целесообразно сменить </span><span style=\" font-family:'Times New Roman';\">диагноз на:</span><span style=\" font-family:'Times New Roman'; font-weight:600;\">_____________________________________________</span><span style=\" font-family:'Times New Roman';\">F</span><span style=\" font-family:'Times New Roman'; font-weight:600;\">__________</span><span style=\" font-family:'Times New Roman';\">.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Заведующий отделением:                                         /____________________/</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Лечащий врач:                                                            /____________________/</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;

        case 9:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt; font-weight:600;\">Протокол заседания ВК </span><span style=\" font-family:'Times New Roman'; font-size:14pt;\">№___</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">смена диагноза / установления диспансерного наблюдения</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Дата:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Ф.И.О.: $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Возраст: $BIRTHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Адрес: $ADRESS$</span></p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Социальный статус:  инвалид /не инвалид, ___________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Цель представления на ВК:  смена диагноза/установление диспансерного наблюдения.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Дополнения к анамнезу, жалобам, психическому статусу:  ______________________________________________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Психический статус: ____________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Решение ВК: Установить диагноз: ____________________________________. Учитывая наличие хронического, затяжного психического расстройства с тяжелыми стойкими болезненными проявлениями, установить диспансерное наблюдение согласно ст.27 п.1  Закона РФ №3185-1 от 02.07.1992г. </span></p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Председатель ВК:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Члены ВК:</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 10:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt; font-weight:600;\">Протокол заседания ВК </span><span style=\" font-family:'Times New Roman'; font-size:14pt;\">№___</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">смена диагноза / снятие с диспансерного наблюдения</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дата:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Ф.И.О.: $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Возраст: $BITHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Адрес: $ADRESS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Социальный статус:  инвалид /не инвалид, ___________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Цель представления на ВК:  смена диагноза/снятие с диспансерного наблюдения.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Психический статус: ____________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дополнения к анамнезу, психическому статусу:  __________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Решение ВК: Диагноз: _____________________________________________. Учитывая стойкую ремиссию хронического, затяжного психического расстройства с тяжелыми стойкими болезненными проявлениями, снять с диспансерного наблюдения согласно ст.27 п.1  Закона РФ №3185-1 от 02.07.1992г. и назначить ЛП(лечебную помощь).</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Председатель ВК:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Члены ВК:</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 11:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Дата:_________                     Эпикриз</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman'; font-weight:600;\">на ВК</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                 для изменения диагноза и прекращения Д-наблюдения</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Пациент </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\"></span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">$FIO$</span><span style=\" font-family:'Times New Roman';\"> находится на Д-наблюдении с ______ года                  Диагноз: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\"></span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">$DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Инвалидность: инвалид / не инвалид</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечился в психиатрических стационарах (даты), последняя выписка (от какого числа, с каким диагнозом): _____________________________________________________________________________  Получал лечение: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">____(препараты)_____.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">В клинике заболевания: __________________________________________________________________________________________________________________________________________________________. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Психический статус: __________________________________________________________________________________________________________________________________________________________.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Учитывая наличие стойкой ремиссии хронического (затяжного) психического расстройства с тяжёлыми, стойкими (часто обостряющимися) болезненными проявлениями, согласно ст.27 п.1 Закона «О психиатрической помощи и гарантиях прав граждан при её оказании» больной переводится на ЛП(лечебную помощь).</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Заведующий отделением:                                         /____________________/</span> <span style=\" font-family:'Times New Roman';\">            Лечащий врач:                                                            /____________________/</span></p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 12:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Дата:_________                     Эпикриз</span><span style=\" font-family:'Times New Roman';\"> </span><span style=\" font-family:'Times New Roman'; font-weight:600;\">на ВК</span><span style=\" font-family:'Times New Roman';\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                 для изменения диагноза и установления Д-наблюдения</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Получает ЛП с ______ года   Диагноз: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">$</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Инвалидность: инвалид / не инвалид</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Лечился в психиатрических стационарах (даты), последняя выписка (от какого числа, с каким диагнозом): _____________________________________________________________________________  Получает поддерживающее лечение: </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">____(препараты)_____.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">В клинике заболевания: __________________________________________________________________________________________________________________________________________________________. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Психический статус: __________________________________________________________________________________________________________________________________________________________.Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Учитывая наличие хронического (затяжного) психического расстройства с тяжёлыми, стойкими (часто обостряющимися) болезненными проявлениями, согласно ст.27 п.1 Закона «О психиатрической помощи и гарантиях прав граждан при её оказании» больной нуждается в установлении диспансерного наблюдения, о чём пациенту сообщено.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Заведующий отделением:                                         /____________________/</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Лечащий врач:                                                            /____________________/</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 17:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; font-size:14pt; font-weight:600;\">Протокол заседания ВК №___</span> </p>\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri';\">Повторное / Первичное представление на МСЭ</span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Дата: ________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Ф.И.О.: </span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\"></span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">$FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Возраст: </span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">$BIRTHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Адрес: </span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">$ADRESS$</span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Социальный статус: ________________________________________________________ Группа инвалидности с ___________ года</span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">     </span></p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Цель представления на ВК:    </span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">Представление на МСЭ для определения/продления группы инвалидности</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Анамнез (динамика заболевания в межкомиссионный период): ______________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Психический статус при направлении на МСЭ: </span><span style=\" color:#000000;\">____________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Решение ВК:  ________________________________________________________________ </span></p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Председатель ВК:</span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">    </span></p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Calibri'; color:#000000;\">Члены ВК:</span><span style=\" font-family:'Calibri'; text-decoration: underline; color:#000000;\">      </span></p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 18:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">(Число, месяц, год)</span><span style=\" font-weight:600;\">.                             Эпикриз на МСЭ</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Наблюдается у врача-психиатра с ______года.    Диагноз: <span style=\" text-decoration: underline;\">$DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Инвалидность    <span style=\" text-decoration: underline;\">(из программы)</span> группа    до <span style=\" text-decoration: underline;\">(дата очередного освидетельствования из программы)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Лечение  в ПБ:   _________________________________________________________                      </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Поддерживающее лечение: <span style=\" text-decoration: underline;\">(препараты из программы) </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">В клинической картине (адаптивные возможности,  квалификация психического состояния, тип течения настоящего заболевания, обоснование направления на МСЭ): __________________________________________________________________________________________________________________________________________________________Психическое состояние: __________________________________________________________________________________________________________________________________________________________ </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Диагноз<span style=\" text-decoration: underline;\">: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">            </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Нуждается в продлении/установлении группы инвалидности.           </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">                        Заведующая отделением:                                 /___________________/ </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">                        Лечащий врач:                                                   /___________________/ </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 23:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"0\">\n<tr>\n<td width=\"336\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"data:image/jpeg;base64,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\" width=\"74\" height=\"83\" style=\"vertical-align: top;\" /> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">          ПРАВИТЕЛЬСТВО санкт-петербурга                        </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">                            АДМИНИСТРАЦИЯ                               </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">Выборгского района <br />Санкт-Петербурга</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Санкт-Петербургское </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">государственное бюджетное учреждение здравоохранения</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">«Психоневрологический диспансер №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">СПб ГБУЗ «ПНД №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Фермское шоссе, д. 34, Санкт-Петербург, 197341</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Тел./Факс (812) 302-11-01</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">E-mail: </span><a href=\"mailto:pnd2@zdrav.spb.ru\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-decoration: underline; color:#0000ff;\">pnd2@zdrav.spb.ru</span></a><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">                    ИНН/КПП 7802091710/781401001</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          От __\u00AD\u00AD\u00AD\u00AD___________     №______________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          На №____________     от   ______________</span><span style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; text-decoration: underline;\">               </span></p></td>\n<td width=\"319\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Куда:_______________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Кому:_______________________  </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p></td></tr></table>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:16pt;\"> </span> </p>\n<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">   На Ваш запрос от __________ №__________ (Вх. СПб ГБУЗ «ПНД №2» №_____ от __________) в связи с ____</span><span style=\" font-family:'Times New Roman'; font-size:14pt; text-decoration: underline;\">(рассмотрением материала КУСП-8649 от 20.12.2015г. или другое)</span><span style=\" font-family:'Times New Roman'; font-size:14pt;\">_________ сообщаем, что гр. $FIO$, $BIRTHDAY$ г.р., проживающий(ая) по адресу:  $ADRESS$ , состоит на диспансерном учете в СПб ГБУЗ «Психоневрологический диспансер №2» с __</span><span style=\" font-family:'Times New Roman'; font-size:14pt; text-decoration: underline;\">(даты)</span><span style=\" font-family:'Times New Roman'; font-size:14pt;\">______ с диагнозом: $DIAGNOS$ .</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">       </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Заместитель главного врача                                                                                        по медицинской части                                                           А.Н.Саренко                                                                                                                                                                                                                                                                                                                                                                                                                                                                              </span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 24:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<table border=\"0\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"0\">\n<tr>\n<td width=\"336\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\"data:image/jpeg;base64,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\" width=\"74\" height=\"83\" style=\"vertical-align: top;\" /> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">          ПРАВИТЕЛЬСТВО санкт-петербурга                        </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">                            АДМИНИСТРАЦИЯ                               </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-transform:uppercase;\">Выборгского района <br />Санкт-Петербурга</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Санкт-Петербургское </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">государственное бюджетное учреждение здравоохранения</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">«Психоневрологический диспансер №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">СПб ГБУЗ «ПНД №2»</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Фермское шоссе, д. 34, Санкт-Петербург, 197341</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">Тел./Факс (812) 302-11-01</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">E-mail: </span><a href=\"mailto:pnd2@zdrav.spb.ru\"><span style=\" font-family:'Times New Roman'; font-size:10pt; text-decoration: underline; color:#0000ff;\">pnd2@zdrav.spb.ru</span></a><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">                    ИНН/КПП 7802091710/781401001</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          От __\u00AD\u00AD\u00AD\u00AD___________     №______________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">          На №____________     от   ______________</span><span style=\" font-family:'Times New Roman'; font-size:10pt; font-weight:600; text-decoration: underline;\">               </span></p></td>\n<td width=\"319\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Куда:_______________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Кому:_______________________  </span></p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p></td></tr></table>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:16pt;\"> </span> </p>\n<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">На Ваш запрос от ___________г. (Вх.СПб ГБУЗ «ПНД№2» №___ от ________г.) сообщаем, что в соответствии со ст. 8,9 Закона РФ от 02.07.1992 N 3185-1 &quot;О психиатрической помощи и гарантиях прав граждан при ее оказании&quot; «Сведения о факте обращения гражданина за психиатрической помощью, состоянии его психического здоровья и диагнозе психического расстройства, иные сведения, полученные при оказании ему психиатрической помощи, составляют врачебную тайну, охраняемую </span><a href=\"consultantplus://offline/ref=778F0029EFCD563DBFA19BC7F87F1A140378012D55F8E239F44D5EB1A5CCADDC7E5FF9960527073BvFH\"><span style=\" font-family:'Times New Roman'; font-size:14pt; text-decoration: underline; color:#000000;\">законом</span></a><span style=\" font-family:'Times New Roman'; font-size:14pt;\">» и требование любых сведений о состоянии здоровья граждан из психиатрических учреждений, кроме получения таких сведений судом, органами следствия и дознания, запрещено. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">      На основании вышеизложенного сообщить Вам сведения в отношении ______________________________________________________ невозможно.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">       </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">                                       </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt;\">Заместитель главного врача                                                                                                  по медицинской части                                                            А.Н. Саренко</span> </p></td></tr></table></body></html>";

            break;
        case 25:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:16pt;\">Согласие</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Я,____________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:9pt;\">                                                                              Ф.И.О. заполняется собственноручно</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дата рождения___(из программы)___г.р., зарегистрированный(ая)  по адресу: ___(из программы)___,</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">паспорт ___(из программы)___выдан___(из программы)___,</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">разрешаю сообщать информацию о моем психическом здоровье и иные сведения, содержащие врачебную тайну о моем здоровье, а также отоваривать рецепты на необходимые мне лекарства моему/моей _______________________</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">ФИО,_______________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:9pt;\">                                                                              Ф.И.О. заполняется собственноручно</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">паспорт серия ______ № __________, выдан _______________________________________,</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">и считать указанного мною человека моим доверенным лицом в СПб ГБУЗ «ПНД №2».</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">_________________г.                                         _________________/_____________________/</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                                                           </span><span style=\" font-family:'Times New Roman'; font-size:9pt;\">подпись                                Ф.И.О.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p></td></tr></table></body></html>";
            break;
        case 26:
            body_doc ="<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:16pt;\">Согласие</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Я,____________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:9pt;\">                                                                              Ф.И.О. заполняется собственноручно</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дата рождения___(из программы)___г.р., зарегистрированный(ая)  по адресу: ___(из программы)___,</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">паспорт ___(из программы)___выдан___(из программы)___,</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">разрешаю сообщать информацию о моем психическом здоровье и иные сведения содержащие врачебную тайну о моем здоровье в адрес Жилищного отдела Администрации __________________________________ района.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">_________________г.                                         _________________/_____________________/</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">                                                                                           </span><span style=\" font-family:'Times New Roman'; font-size:9pt;\">подпись                                Ф.И.О.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p></td></tr></table></body></html>" ;
            break;
        case 27:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt; text-decoration: underline;\">Протокол заседания ВК</span><span style=\" font-family:'Times New Roman'; font-size:14pt;\">  №___</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">на выдачу справки по форме №135 а-р                                                                                 (приложение №1 к распоряжению Комитета по здравоохранению от 30.03.2007)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дата: ____________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Ф.И.О.:  $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Возраст: $BIRTHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Адрес: $ADRESS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Социальный статус:  ___________________________________________________________.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Диагноз: $DIAGNOS$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Цель представления на ВК:  решение вопроса о выдаче справки на жилищную комиссию по форме №135а-р (приложение №1 к распоряжению Комитета по здравоохранению от 30.03.2007)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Дополнения к анамнезу, жалобам, психическому статусу: </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:10pt;\">_____________________________________________________________________________________________ </span><span style=\" font-family:'Times New Roman';\">Психический статус:</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\"> _____________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">Решение ВК:</span><span style=\" font-family:'Times New Roman';\"> на основании освидетельствования</span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">,</span><span style=\" font-family:'Times New Roman';\"> медицинских документов, комиссия установила, что пациент страдает заболеванием, входящим  в Перечень тяжелых форм заболеваний, при которых невозможно совместное проживание граждан в одной квартире, утвержденный постановлением Правительства РФ от 16.06.2006 г. №378 «Об утверждении перечня тяжелых форм хронических заболеваний, при которых невозможно совместное проживание граждан в одной квартире».</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Председатель ВК:</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Члены ВК:</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 28:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"right\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" font-size:10pt; color:#000000;\">ПРИЛОЖЕНИЕ 1 <br />к распоряжению Комитета по здравоохранению <br />от 30.03.2007 N 135а-р<br /><br /></span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">¦Штамп учреждения¦</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">СПРАВКА</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">N ___________от &quot;___&quot;__________ 20__ года</span> </p>\n<p align=\"center\" style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">(номер справки соответствует номеру регистрации по журналу формы N 035/у-02)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">  </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">    Справка  выдана в целях принятия решения о приеме на учет граждан в  качестве нуждающихся в жилых помещениях в соответствии с </span><a href=\"http://www.bestpravo.ru/federalnoje/ea-akty/i2a.htm\"><span style=\" color:#000000;\">пунктом 4</span></a> части  1  статьи  51  Жилищного  кодекса  Российской Федерации и(или) предоставлении  жилого   помещения   по  договору  социального  найма в соответствии с <a href=\"http://www.bestpravo.ru/federalnoje/ea-akty/i2a.htm\"><span style=\" color:#000000;\">пунктом 3 части 2 статьи 57</span></a><span style=\" color:#000000;\"> Жилищного кодекса РФ</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">Гражданину (гражданке) $FIO$</span> <span style=\" color:#000000;\">                                                                     \t\t\t\t</span><span style=\" font-size:9pt; color:#000000;\">(Ф.И.О. полностью)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">$PASSPORT$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" font-size:9pt; color:#000000;\">                 (серия, номер паспорта, кем и когда выдан)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">Дата рождения $BIRTHDAY$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">Зарегистрированному(ой) по адресу: $ADRESS$ На  основании  освидетельствования гр. $FIO$, медицинских  документов  комиссия  установила, что  он (она) страдает заболеванием, которое входит в </span><a href=\"http://www.bestpravo.ru/federalnoje/iw-zakony/p6a.htm\"><span style=\" color:#000000;\">Перечень</span></a><span style=\" color:#000000;\"> тяжелых форм заболеваний, при которых  невозможно  совместное  проживание граждан в одной квартире, утвержденный  постановлением Правительства РФ от 16.06.2006 N 378 &quot;Об утверждении перечня тяжелых форм хронических заболеваний, при которых невозможно совместное проживание граждан в одной квартире&quot;.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\">Председатель комиссии __________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">                                 (Ф.И.О., подпись)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">Члены комиссии: ________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">                                 (Ф.И.О., подпись)</span> \t\t  <span style=\" color:#000000;\">\t\t\t_______________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">                                 (Ф.И.О., подпись)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" color:#000000;\">Главный врач     ___________________________________________________                               (подпись и круглая печать учреждения)</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#000000;\"><br /> </span><span style=\" font-size:10pt; color:#000000;\">Примечание. Справка  действительна  в течение трех месяцев с даты выдачи.</span>  </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 29:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Дата________________      Эпикриз</span><span style=\" font-family:'Times New Roman';\"> на ВК для  выдачи справки №135 а-р</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Ф.И.О.: $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Наследственность _____________. Родился(ась) в _________________, ____________ ребенок в семье. Родители __________________. Раннее развитие без особенностей. Учился(ась) ________________, окончил(а) _______ классов, _____________. С _____ по ____ работал(а) продавцом. В настоящее время проживает с дочерью и матерью в однокомнатной квартире, отношения в семье эпизодически бывают напряженными, что определяется, в том числе, заболеванием пациента(ки). Под наблюдением психиатров с ______ года. Неоднократно госпиталировался(ась) в психитарический стационар, дневной стационар СПб ГБУЗ «ПНД №2». В дебюте заболевания ______. Поддерживающую терапию получает регулярно. На сегодняшний день клиническая картина болезни представлена, в первую очередь, ______________.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\"> Психический статус: __________________________________________________________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Диагноз: ___(из программы)___</span> </p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            На основании освидетельствования пациента и/или медицинской документации представить на ВК для выдачи справки 135 а-р, т.к. его заболевание входит в Перечень тяжёлых форм заболеваний, при которых невозможно совместное проживание граждан в одной квартире, утвержденный постановлением Правительства РФ от 16.06.2006 г. № 378</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Заведующий отделением:                                                /___________________/</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">            Лечащий врач:                                                                  /___________________/</span> </p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        case 35:
            body_doc = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'Courier'; font-size:12pt; font-weight:400; font-style:normal;\">\n<table border=\"0\" style=\"-qt-table-type: root; margin-top:56.693px; margin-bottom:56.693px; margin-left:56.693px; margin-right:56.693px;\">\n<tr>\n<td style=\"border: none;\">\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt; font-weight:600;\">Протокол заседания ВК №  </span><span style=\" font-size:16pt; text-decoration: underline;\">         </span> </p>\n<p align=\"center\" style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">на изменение диагноза</span><span style=\" font-size:11pt;\">.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\"> </span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\">Дата:</span><span style=\" font-size:11pt;\">  </span><span style=\" font-size:11pt; text-decoration: underline;\">______________</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\">Ф.И.О: $FIO$</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\">Возраст</span><span style=\" font-size:11pt;\">: $BIRTHDAY$ г.р.</span> </p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\">Адрес</span><span style=\" font-size:11pt;\">:   $ADRESS$</span></p>\n<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-size:11pt; font-weight:600;\">Социальный статус</span><span style=\" font-size:11pt;\">:</span><span style=\" font-size:11pt; text-decoration: underline;\">       инвалид не имеет, не работает. </span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-weight:600;\">Диагноз</span>:<span style=\" font-family:'Times New Roman';\"> $DIAGNOS$</span></p>\n<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; line-height:150%;\"><span style=\" font-weight:600;\">Цель представления на ВК</span>:  <span style=\" text-decoration: underline;\">изменение диагноза.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" font-weight:600;\">Дополнения к анамнезу. </span><span style=\" font-family:'Times New Roman'; font-weight:600;\">Динамика заболевания:</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">Наследственность ______________________. Родился в ______________. В родах _______________________. Отмечалось некоторое отставание от сверстников в психофизическом развитии.  В школу пошел с _______ лет, с программой __________________, окончил _____ классов и ________ по специальности _____________________. По специальности ____________. В армии _____________. Семейный статус:______________________. Эпид.анамнез: ____________________________. Аллергоанамнез: ____________________. Вредные привычки: __________________________.  </span></p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman';\">_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-weight:600;\">Объективно: </span><span style=\" font-family:'Times New Roman';\">_______________________________________________________________________________________________________________________________________________________________.</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" font-family:'Times New Roman';\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\"><span style=\" font-weight:600;\">         </span><span style=\" font-weight:600;\">Решение ВК:</span> <span style=\" font-family:'Times New Roman'; text-decoration: underline;\">Исходя из данных анамнеза, клинической картины, дополнительных методов обследования целесообразно сменить диагноз на:</span><span style=\" font-family:'Times New Roman'; font-weight:600; text-decoration: underline;\">______________. </span><span style=\" font-family:'Times New Roman'; text-decoration: underline;\">F ________</span> </p>\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; background-color:#ffffff;\">  </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">         Председатель ВК:</span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\"> </span> </p>\n<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">Члены ВК:</span><span style=\" font-size:11pt; text-decoration: underline;\">                                                                                             </span></p></td></tr></table></body></html>";
            if(db.open())
            {
                query.exec("SELECT \
                           all_view_patient.id, \
                           concat(all_view_patient.fname, ' ',\
                           all_view_patient.name, ' ',\
                           all_view_patient.mname), \
                           all_view_patient.birthday, \
                           concat(street.name, \
                                            CASE\
                                              WHEN address_patient.building ='' THEN ''\
                                              ELSE ' корп.'\
                                            END , \
                                            address_patient.building, ' д.',\
                                            address_patient.home, ' кв.',\
                                            address_patient.flat) AS Street,\
                           diagnos.code  \
                         FROM \
                           test.all_view_patient, \
                           test.address_patient, \
                           test.street, \
                           test.diagnos_patient, \
                           test.diagnos\
                         WHERE \
                           all_view_patient.id = diagnos_patient.medcard_id AND\
                           address_patient.medcard_id = all_view_patient.id AND\
                           street.id = address_patient.street_id AND\
                           diagnos.id = diagnos_patient.diagnos_id AND diagnos_patient.delete_row = false AND diagnos_patient.id_parent is Null AND all_view_patient.id ="+global_id_str);
                        while(query.next())
                            {
                                body_doc.replace("$FIO$",query.value(1).toString());
                                body_doc.replace("$BIRTHDAY$",query.value(2).toDate().toString("dd.MM.yyyy"));
                                body_doc.replace("$ADRESS$",query.value(3).toString());
                                body_doc.replace("$DIAGNOS$",query.value(4).toString());

                            }
                        }
            break;
        }
    }

    body_doc.replace("'","''");

    if(db.open())
    {
        QSqlQuery query;
        query.exec("INSERT INTO test.text_info_patients(medcard_id, date_creat, staff_add_id, delete_row, doc_id, text) VALUES ("+global_id_str+", '"+QDate::currentDate().toString("dd.MM.yyyy")+"', "+global_staff_id+", 'false', "+id_document+", '"+body_doc+"') RETURNING id");
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
    ui->comboBox_department->clear();
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
        if(db.open())
        {
            query.exec("SELECT id, name  FROM library.departments;");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }
            else
            {
                while (query.next())
                {
                    ui->comboBox_department->addItem(query.value(1).toString(),query.value(0).toString());
                }
            }
        }


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
        query.exec("SELECT id, description FROM test.end order by id");
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
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("1"));
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("11"));
        change_combox_document();
        //ui->comboBox_document->setCurrentIndex(ui->comboBox_document->findData("31"));
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
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("1"));
        ui->comboBox_end->setCurrentIndex(ui->comboBox_end->findData("11"));
        change_combox_document();
        //ui->comboBox_document->setCurrentIndex(ui->comboBox_document->findData("31"));
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
        query.exec("SELECT patient.fname, patient.name, patient.mname, medcard.sex,  patient.serial_passport, patient.number_passport, medcard.birthday, medcard.job_place, medcard.tutor, medcard.pt, medcard.group_lgot_preparat, medcard.area_id, medcard.serial_snils, medcard.district_id, medcard.group_inv_obsh, medcard.polis_serial, medcard.polis_number, patient.vidan, medcard.time_stamp, medcard.department  FROM test.patient, test.medcard  WHERE medcard.patient_id = patient.id AND medcard.id ="+id_str);
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
            QString department = query.value(19).toString();


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
            ui->comboBox_department->setCurrentIndex(ui->comboBox_department->findData(department));
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
    QString department = ui->comboBox_department->currentData().toString();

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
                    query.exec("SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', 99, "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', '"+ds_start+"', NULL, '"+why_remove+"', "+direct_value+", "+direct_from_value+", "+department+")");
                    qDebug()<<"SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', 99, "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', '"+ds_start+"', NULL, '"+why_remove+"', "+direct_value+", "+direct_from_value+", "+department+")";
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                else if(ui->checkBox_ds_end_state->checkState()==Qt::Checked)
                {
                    query.exec("SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', "+why_remove+", "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', NULL, '"+ds_end+"', '"+why_remove+"', -1, -1, "+department+")");
                    qDebug()<<"SELECT * FROM insert_all_info('"+fname_value+"', '"+name_value+"', '"+oname_value+"', '"+serial_passport+"', '"+number_passport+"', 99, "+obj.staff_id+", '"+passport_vidan_value+"', 'f', '"+sex_value+"', '"+date_birthday+"', '"+job_place_value+"', '"+tutor_value+"', '"+pt_value+"', '"+group_lgot_value+"', "+area_value+", "+district_value+", '"+group_inv_value+"', '"+polis_serial_value+"', '"+polis_number_value+"', '"+snils_serial_value+"', "+street_id+", '"+home+"', '"+building+"', '"+flat+"', '"+telefon_value+"', '"+index_street+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', '"+ds_start+"', NULL, '"+why_remove+"', "+direct_value+", "+direct_from_value+", "+department+")";
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
                    query.exec("UPDATE test.medcard SET area_id = '"+area_value+"', sex = "+sex_value+", birthday='"+date_birthday+"', job_place='"+job_place_value+"', tutor='"+tutor_value+"', pt='"+pt_value+"', group_lgot_preparat='"+group_lgot_value+"', serial_snils='"+snils_serial_value+"', polis_number='"+polis_number_value+"', polis_serial = '"+polis_serial_value+"', district_id="+district_value+", group_inv_obsh='"+group_inv_value+"', department="+department+" WHERE id ="+id_str);
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
    model_comments->setFilter("delete_row = 'false' AND type = 0 AND medcard_id = "+global_id_str);
    model_comments->setSort(0,Qt::AscendingOrder);
    model_comments->select();
    if(model_comments->lastError().isValid())
    {
        qDebug()<<model_comments->lastError();
    }


    ui->tableView_comments_patient->setFont(font);
    ui->tableView_comments_patient->setModel(model_comments);

    if(ui->tableView_comments_patient->model()->rowCount() > 0)
    {
        QString num_count;
        num_count.setNum(ui->tableView_comments_patient->model()->rowCount());
        ui->tabWidget->setTabText(2,"Комментарии ("+num_count+")");
    }

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
    ui->tableView_comments_patient->hideColumn(8);

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
    switch (obj.staff_id.toInt()) {
    case 18:
        menu->addAction("Удалить", this, SLOT(del_comments()));
        break;
    case 17:
        menu->addAction("Удалить", this, SLOT(del_comments()));
        break;
    case 14:
        menu->addAction("Удалить", this, SLOT(del_comments()));
        break;
    }
    menu->exec(ui->tableView_comments_patient->mapToGlobal(pos));
}
void Dialog_patient::del_comments()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int selected_tables = ui->tableView_comments_patient->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_comments_patient->currentIndex().row();
        QString id = ui->tableView_comments_patient->model()->index(row,0).data(Qt::DisplayRole).toString();
        if(db.open())
        {
            query.exec("UPDATE test.comments SET delete_row = 'true' WHERE id = "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
            }
            else
            {
                model_comments->select();
            }
        }
    }
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
