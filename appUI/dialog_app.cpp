#include "dialog_app.h"
#include "ui_dialog_app.h"

Dialog_app::Dialog_app(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_app)
{
    ui->setupUi(this);
    Objects_app obj;
    this->setWindowModality(Qt::WindowModal);
    this->setWindowFlags(windowFlags()|Qt::WindowMaximizeButtonHint);
    this->setWindowFlags(windowFlags()|Qt::WindowMinimizeButtonHint);
    if(obj.staff_id == "14")
    {
        ui->pushButton_emsrn->setHidden(false);
        connect(ui->pushButton_emsrn,SIGNAL(clicked()),SLOT(open_emsrn()));
    }
    else
    {
        ui->pushButton_emsrn->setHidden(true);
    }
    connect(ui->pushButton_browser,SIGNAL(clicked(bool)),SLOT(open_browser()));
    connect(ui->pushButton_open_chat,SIGNAL(clicked(bool)),SLOT(open_chat()));
    connect(ui->pushButton_doc_edit,SIGNAL(clicked(bool)),SLOT(open_Document_editor()));
}

Dialog_app::~Dialog_app()
{
    delete ui;
}
void Dialog_app::open_browser()
{
    MainWindow_web_browser *browser = new MainWindow_web_browser();
    browser->setFocus();
    browser->activateWindow();
    browser->raise();
    browser->setWindowModality(Qt::WindowModal);
    browser->showMaximized();
}
void Dialog_app::open_chat()
{
    Dialog_chat dialog;
    dialog.exec();
}
void Dialog_app::open_Document_editor()
{
    Window_Document_Editor *WDE = new Window_Document_Editor();

    WDE->show();
    //WDE->open_document(1,"29");
}
void Dialog_app::open_emsrn()
{
    Dialog_emsr_find dialog;
    dialog.setParam(0,"0");
    dialog.exec();
}
