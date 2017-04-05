#ifndef DIALOG_PAGE_SETTINGS_H
#define DIALOG_PAGE_SETTINGS_H

#include <QDialog>
#include <QSettings>
#include <Objects/User/objects_app.h>

namespace Ui {
class Dialog_page_settings;
}

class Dialog_page_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_page_settings(QWidget *parent = 0);
    ~Dialog_page_settings();

private:
    Ui::Dialog_page_settings *ui;
public slots:
    void load_settings();
    void save_settings();
};

#endif // DIALOG_PAGE_SETTINGS_H
