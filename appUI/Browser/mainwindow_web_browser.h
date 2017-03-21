#ifndef MAINWINDOW_WEB_BROWSER_H
#define MAINWINDOW_WEB_BROWSER_H

#include <QMainWindow>
#include <QWebView>
#include <QWebDatabase>
#include <QWebHistory>

namespace Ui {
class MainWindow_web_browser;
}

class MainWindow_web_browser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_web_browser(QWidget *parent = 0);
    ~MainWindow_web_browser();
    QWebView * webView;

private:
    Ui::MainWindow_web_browser *ui;
public slots:
    void load_web();
    void load_history();
};

#endif // MAINWINDOW_WEB_BROWSER_H
