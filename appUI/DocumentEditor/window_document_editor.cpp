#include "window_document_editor.h"
#include "ui_window_document_editor.h"

Window_Document_Editor::Window_Document_Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window_Document_Editor)
{
    ui->setupUi(this);
    state_change = false;
    ui->menubar->hide();
    setting_ui();
    connect(push_align_center,SIGNAL(clicked(bool)),SLOT(center_align()));
    connect(push_align_left,SIGNAL(clicked(bool)),SLOT(left_align()));
    connect(push_align_right,SIGNAL(clicked(bool)),SLOT(right_align()));
    connect(push_set_text_bold,SIGNAL(clicked(bool)),SLOT(set_text_bold()));
    connect(push_set_text_italic,SIGNAL(clicked(bool)),SLOT(set_text_italic()));
    connect(push_set_text_p,SIGNAL(clicked(bool)),SLOT(set_text_p()));
    connect(push_save,SIGNAL(clicked(bool)),SLOT(save_document()));
    connect(push_print,SIGNAL(clicked(bool)),SLOT(print_sheet()));
    connect(push_settings_page,SIGNAL(clicked(bool)),SLOT(edit_settings_page()));

    connect(ui->action_print,SIGNAL(triggered(bool)),SLOT(print_sheet()));
    connect(ui->action_save,SIGNAL(triggered(bool)),SLOT(save_document()));
    connect(ui->action_param_page,SIGNAL(triggered(bool)),SLOT(edit_settings_page()));

    connect(push_align_justify,SIGNAL(clicked(bool)),SLOT(justify_align()));
    connect(w,SIGNAL(cursorPositionChanged()),SLOT(load_data_to_cursor()));
    connect(font_combo_box,SIGNAL(currentIndexChanged(QString)),SLOT(set_font_family(QString)));
    connect(spinbox,SIGNAL(valueChanged(int)),SLOT(set_font_size(int)));
    connect(w,SIGNAL(selectionChanged()),SLOT(selected_text_changes()));





    connect(push_add_table,SIGNAL(clicked(bool)),SLOT(open_dialog_add_table()));
}

Window_Document_Editor::~Window_Document_Editor()
{
    delete ui;
}

void Window_Document_Editor::closeEvent(QCloseEvent *)
{
    qDebug()<<"start_signal";
    if(state_change)
    {
        warning_save_document();
    }
    emit close_document();

}

void Window_Document_Editor::setting_ui()
{
    w = new PagesTextEdit;
    doc = new QTextDocument;
    push_save = new QPushButton();
    push_print = new QPushButton();
    push_align_center = new QPushButton();
    push_align_left = new QPushButton();
    push_align_right = new QPushButton();
    push_align_justify = new QPushButton();
    push_add_table = new QPushButton();
    push_set_text_bold = new QPushButton();
    push_set_text_italic = new QPushButton();
    push_set_text_p = new QPushButton();
    push_settings_page = new QPushButton();

    push_set_text_bold->setCheckable(true);
    push_set_text_italic->setCheckable(true);
    push_set_text_p->setCheckable(true);

    spinbox = new QSpinBox();

    font_combo_box = new QFontComboBox();
    ui->toolBar->addWidget(push_save);
    ui->toolBar->addWidget(push_print);
    ui->toolBar->addWidget(push_align_left);
    ui->toolBar->addWidget(push_align_center);
    ui->toolBar->addWidget(push_align_right);
    ui->toolBar->addWidget(push_align_justify);
    ui->toolBar->addWidget(push_set_text_bold);
    ui->toolBar->addWidget(push_set_text_italic);
    ui->toolBar->addWidget(push_set_text_p);
    ui->toolBar->addWidget(spinbox);
    ui->toolBar->addWidget(font_combo_box);
    ui->toolBar->addWidget(push_add_table);
    ui->toolBar->addWidget(push_settings_page);

    //push_align_center->setText("centre");
    push_align_center->setIcon(QIcon(":/icon/png/images/center.png"));
    push_align_left->setIcon(QIcon(":/icon/png/images/Left.png"));
    push_align_right->setIcon(QIcon(":/icon/png/images/Right.png"));
    push_align_justify->setIcon(QIcon(":/icon/png/images/justify.png"));
    push_save->setIcon(QIcon(":/icon/png/images/save.png"));
    push_print->setIcon(QIcon(":/icon/png/images/print.png"));
    push_set_text_bold->setIcon(QIcon(":/icon/png/images/1484595950_bold.png"));
    push_set_text_italic->setIcon(QIcon(":/icon/png/images/1484595941_text_italic.png"));
    push_set_text_p->setIcon(QIcon(":/icon/png/images/1484596168_text-format.png"));
    push_add_table->setIcon(QIcon(":/icon/png/images/1484596761_098_Spreadsheet.png"));
    push_settings_page ->setText("Параметры страницы");


    spinbox->setValue(12);

    w->setDocument(doc);

    load_settings_page();
    w->resize(600, 400);
    w->setUpdatesEnabled(true);
    doc->setDefaultFont(QFont("Arial", 12));
    ui->verticalLayout_3->addWidget(w);
    w->show();


}
void Window_Document_Editor::load_settings_page()
{
    Objects_app obj;
    QSettings *settings = new QSettings(obj.path_settings,QSettings::IniFormat);
    int format_page = settings->value("Document/list_format").toInt();
    int MarL = settings->value("Document/MarL").toInt();
    int MarR = settings->value("Document/MarR").toInt();
    int MarB = settings->value("Document/MarB").toInt();
    int MarT = settings->value("Document/MarT").toInt();

    switch (format_page) {
    case 0:
        w->setPageFormat(QPageSize::A0);
        break;
    case 1:
        w->setPageFormat(QPageSize::A1);
        break;
    case 2:
        w->setPageFormat(QPageSize::A2);
        break;
    case 3:
        w->setPageFormat(QPageSize::A3);
        break;
    case 4:
        w->setPageFormat(QPageSize::A4);
        break;
    case 5:
        w->setPageFormat(QPageSize::A5);
        break;
    case 6:
        w->setPageFormat(QPageSize::A6);
        break;
    default:
        w->setPageFormat(QPageSize::A4);
        break;
    }
    if(MarL == 0 && MarT == 0 && MarB == 0 && MarR == 0)
    {
        MarL = 15;
        MarR = 15;
        MarB = 15;
        MarT = 15;
    }
    w->setPageMargins(QMarginsF(MarL, MarT, MarR, MarB));
    w->setUsePageMode(true);
    w->setPageNumbersAlignment(Qt::AlignTop | Qt::AlignHCenter);
    w->textCursor().setPosition(old_text_cursor_pos);
    w->insertPlainText("/");




}

void Window_Document_Editor::left_align()
{
    w->setAlignment(Qt::AlignLeft);
}
void Window_Document_Editor::center_align()
{
    w->setAlignment(Qt::AlignCenter);
}
void Window_Document_Editor::right_align()
{
    w->setAlignment(Qt::AlignRight);
}
void Window_Document_Editor::justify_align()
{
    w->setAlignment(Qt::AlignJustify);
}

void Window_Document_Editor::print_sheet()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        w->print(&printer);
    }
    delete dlg;
}
void Window_Document_Editor::load_data_to_cursor()
{
    if(block_changet_font)
    {
        spinbox->setValue(w->currentCharFormat().font().pointSize());
        font_combo_box->setCurrentIndex(font_combo_box->findText(w->currentCharFormat().font().family()));
    }


}
void Window_Document_Editor::set_font_size(int x)
{

    w->setFontPointSize(x);
    block_changet_font = true;
}
void Window_Document_Editor::set_text_bold()
{
    w->setFontWeight(push_set_text_bold->isChecked() ? 600 : 400);

}
void Window_Document_Editor::set_text_italic()
{
    w->setFontItalic(push_set_text_italic->isChecked());
}
void Window_Document_Editor::set_text_p()
{
    w->setFontUnderline(push_set_text_p->isChecked());
}

void Window_Document_Editor::set_font_family(QString family_font)
{
    w->setFontFamily(family_font);
    block_changet_font = true;
}
void Window_Document_Editor::selected_text_changes()
{
    block_changet_font = false;
    push_set_text_bold->setChecked(false);
    push_set_text_italic->setChecked(false);
    push_set_text_p->setChecked(false);

}
void Window_Document_Editor::view_html()
{
    qDebug()<<doc->toHtml();
}
void Window_Document_Editor::open_dialog_add_table()
{
    old_text_cursor_pos =  w->textCursor().position();
    Dialog_table_add *dialog= new Dialog_table_add() ;
    connect(dialog,SIGNAL(table(int,int)),SLOT(add_table(int,int)));
    dialog->exec();

}
void Window_Document_Editor::add_table(int x,int y)
{
    QString table_create;
    table_create.append("<table border=\"1\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;\" cellspacing=\"0\" cellpadding=\"0\">");
    for(int i = 0;i<=x;i++)
    {
        table_create.append("<tr>");
        for(int j = 0;j<=y;j++)
        {
            table_create.append("<td width=\"128\" style=\" vertical-align:top; padding-left:0; padding-right:0; padding-top:0; padding-bottom:0;\"></td>");
        }
        table_create.append("</tr>");
    }
    table_create.append("</table>");
    w->textCursor().setPosition(old_text_cursor_pos);
    w->insertHtml(table_create);

}
void Window_Document_Editor::open_document(int x,QString id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    id_text = id;
    switch (x) {
    case 0:

        break;
    case 1:
        if(db.open())
        {
            query.exec("SELECT id, medcard_id, type, date_creat, staff_add_id, text, delete_row, doc_id, time_stamp FROM test.text_info_patients WHERE id="+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
            while(query.next())
            {
                QString text = query.value(5).toString();
                text.replace("''","'");
                doc->setHtml(text);

            }
            }
        }
        state_change = false;
        break;
    }
    connect(w,SIGNAL(textChanged()),SLOT(text_change()));
}
void Window_Document_Editor::text_change()
{
    state_change = true;
}
void Window_Document_Editor::warning_save_document()
{
    int ret = QMessageBox::warning(this, tr("Сохранение"),
                                   tr("Текст был изменен! \nСохранить изменения в тексте?"),
                                   QMessageBox::Yes|QMessageBox::No);
    if(ret==16384)
    {
        save_document();
    }
    if(ret==65536)
    {
        state_change = false;
    }
}
void Window_Document_Editor::save_document()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString text = doc->toHtml();

    text.replace("'","''");
    text.replace("\n", "");
    if(db.open())
    {

        query.exec("UPDATE test.text_info_patients SET date_creat = '"+date+"', staff_add_id = "+obj.staff_id+", text='"+text+"' WHERE text_info_patients.id ="+id_text);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        else
        {
            QMessageBox::information(this,"Выполнено","Документ был успешно сохранен");
        }

    }
    state_change = false;
}
void Window_Document_Editor::edit_settings_page()
{
    old_text_cursor_pos =  w->textCursor().position();
    Dialog_page_settings dialog;
    if(dialog.exec())
    {
        load_settings_page();
    }
}

