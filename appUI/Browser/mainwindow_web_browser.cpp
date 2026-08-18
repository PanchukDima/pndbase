#include "mainwindow_web_browser.h"
#include "ui_mainwindow_web_browser.h"

MainWindow_web_browser::MainWindow_web_browser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_web_browser)
{
    ui->setupUi(this);
    webView = new QTextBrowser();
    ui->verticalLayout_2->addWidget(webView);
    connect(ui->pushButton_load,SIGNAL(clicked(bool)),SLOT(load_web()));
}

MainWindow_web_browser::~MainWindow_web_browser()
{
    delete ui;
}
void MainWindow_web_browser::load_web()
{
    QUrl url = "http://"+ui->comboBox->currentText();
    webView->setFocus();
    webView->setSource(url);
    webView->show();
}
void MainWindow_web_browser::load_history()
{

    // QTextBrowser keeps its own navigation history.
    webView->show();
}
