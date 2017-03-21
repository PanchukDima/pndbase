#ifndef DIALOG_BROWSER_H
#define DIALOG_BROWSER_H

#include <QDialog>
#include <QtWebKitWidgets>

namespace Ui {
class Dialog_Browser;
}

class Dialog_Browser : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Browser(QWidget *parent = 0);
    ~Dialog_Browser();
    QWebView * webView;

private:
    Ui::Dialog_Browser *ui;
public slots:
    void load_web();
    void load_history();
};

#endif // DIALOG_BROWSER_H
