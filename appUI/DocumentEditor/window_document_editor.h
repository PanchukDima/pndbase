#ifndef WINDOW_DOCUMENT_EDITOR_H
#define WINDOW_DOCUMENT_EDITOR_H

#include <QMainWindow>
#include "viewer/PagesTextEdit.h"
#include <QFontComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QPrintDialog>
#include <QPrinter>
#include <QDebug>
#include "dialogs/dialog_table_add.h"
#include "dialogs/dialog_page_settings.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCloseEvent>
#include "Objects/User/objects_app.h"
#include <QMessageBox>
#include <QDate>
#include <QSettings>

namespace Ui {
class Window_Document_Editor;
}

class Window_Document_Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window_Document_Editor(QWidget *parent = 0);
    ~Window_Document_Editor();
    PagesTextEdit *w;
    QPushButton *push_save;
    QPushButton *push_print;
    QPushButton *push_align_left;
    QPushButton *push_align_right;
    QPushButton *push_align_center;
    QPushButton *push_align_justify;
    QPushButton *push_add_table;
    QPushButton *push_set_text_bold;
    QPushButton *push_set_text_italic;
    QPushButton *push_set_text_p;
    QPushButton *push_settings_page;

    QFontComboBox *font_combo_box;

    QSpinBox *spinbox;
    QTextDocument* doc;
    int old_text_cursor_pos;
    bool block_changet_font;

    int type_load;
    QString id_text;

    bool state_change;

signals:
    void close_document();

protected:
      virtual void closeEvent(QCloseEvent *event);

private:
    Ui::Window_Document_Editor *ui;
public slots:
    void setting_ui();
    void left_align();
    void center_align();
    void right_align();
    void print_sheet();
    void justify_align();
    void load_data_to_cursor();
    void set_font_size(int);
    void set_font_family(QString);
    void selected_text_changes();
    void open_dialog_add_table();
    void add_table(int,int);
    void view_html();
    void open_document(int, QString);
    void set_text_bold();
    void set_text_italic();
    void set_text_p();
    void text_change();
    void warning_save_document();
    void save_document();
    void load_settings_page();
    void edit_settings_page();
};

#endif // WINDOW_DOCUMENT_EDITOR_H
