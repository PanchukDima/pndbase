#include "dialog_print_preview.h"
#include "ui_dialog_print_preview.h"

Dialog_print_preview::Dialog_print_preview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_print_preview)
{
    ui->setupUi(this);
    webView = new QWebView();
    ui->verticalLayout->addWidget(webView);
}

Dialog_print_preview::~Dialog_print_preview()
{
    delete ui;
}
