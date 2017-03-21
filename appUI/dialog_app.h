#ifndef DIALOG_APP_H
#define DIALOG_APP_H

#include <QDialog>
#include <appUI/Browser/dialog_browser.h>
#include <appUI/Chat/dialog_chat.h>
#include <appUI/DocumentEditor/window_document_editor.h>
#include <appUI/Browser/mainwindow_web_browser.h>

namespace Ui {
class Dialog_app;
}

class Dialog_app : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_app(QWidget *parent = 0);
    ~Dialog_app();

private:
    Ui::Dialog_app *ui;
public slots:
    void open_browser();
    void open_chat();
    void open_Document_editor();
};

#endif // DIALOG_APP_H
