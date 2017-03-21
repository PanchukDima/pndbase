#include "dialog_browser.h"
#include "ui_dialog_browser.h"

Dialog_Browser::Dialog_Browser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Browser)
{
    ui->setupUi(this);
    webView = new QWebView();
    webView->show();
    ui->verticalLayout_2->addWidget(webView);
    connect(ui->pushButton,SIGNAL(clicked(bool)),SLOT(load_web()));
    connect(ui->pushButton_menu,SIGNAL(clicked(bool)),SLOT(load_history()));
}

Dialog_Browser::~Dialog_Browser()
{
    delete ui;
}
void Dialog_Browser::load_web()
{
    QUrl url = "http://"+ui->lineEdit->text();
    webView->setFocus();
    webView->load(url);
    webView->show();
}
void Dialog_Browser::load_history()
{
    webView->page()->history();
    webView->show();
}

