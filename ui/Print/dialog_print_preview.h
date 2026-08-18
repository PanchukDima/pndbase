#ifndef DIALOG_PRINT_PREVIEW_H
#define DIALOG_PRINT_PREVIEW_H

#include <QDialog>
#include <QTextBrowser>

namespace Ui {
class Dialog_print_preview;
}

class Dialog_print_preview : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_print_preview(QWidget *parent = 0);
    ~Dialog_print_preview();
    QTextBrowser * webView;
    QString head_html;
    QString body_html;
    QString footer_html;

private:
    Ui::Dialog_print_preview *ui;
public slots:

};

#endif // DIALOG_PRINT_PREVIEW_H
