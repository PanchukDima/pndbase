#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T17:57:38
#
#-------------------------------------------------

QT       += core gui sql printsupport webkitwidgets network

win32{
QT += axcontainer
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = base
TEMPLATE = app
#CONFIG += CONSOLE
CONFIG+=exceptions
CONFIG+=rtti

QMAKE_CXXFLAGS += -std=c++0x

#TRANSLATIONS = BDPP_ru.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_patient.cpp \
    dialog_settings_user.cpp \
    dialog_login.cpp \
    dialog_registratura.cpp \
    dialog_reports.cpp \
    dialog_add_dynamic_view.cpp \
    dialog_add_visits.cpp \
    dialog_preview_print.cpp \
    new_changes_thread.cpp \
    work_info_patient/diagnos/dialog_diagnos_patient.cpp \
    dialog_hospitalization.cpp \
    dialog_copy_files_to_server.cpp \
    dialog_invalids_patient.cpp \
    dialog_find_filters.cpp \
    add_invalid_class.cpp \
    logs_save.cpp \
    models/model_main_table.cpp \
    models/model_find_table.cpp \
    dialog_edit_print.cpp \
    ui/Print/dialog_print_preview.cpp \
    appUI/dialog_app.cpp \
    appUI/Browser/dialog_browser.cpp \
    network/broadcastlog.cpp \
    appUI/Chat/dialog_chat.cpp \
    work_info_patient/diagnos/dialog_add_diagnos_and_dop_diagnos.cpp \
    work_info_patient/suicide/dialog_suicide.cpp \
    work_info_patient/list_not_work/dialog_list_not_work.cpp \
    work_info_patient/preparate/dialog_preparate.cpp \
    work_info_patient/day_stationar/dialog_day_stationar.cpp \
    work_info_patient/ood/dialog_ood.cpp \
    Objects/User/objects_app.cpp \
    journal/dialog_rvk.cpp \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal.cpp \
    Other_dialog_windows/dialog_fast_find_patient_id.cpp \
    Other_dialog_windows/dialog_diagnos_selected_patient.cpp \
    models/model_dynamic_view.cpp \
    models/model_diagnos_filter.cpp \
    appUI/DocumentEditor/window_document_editor.cpp \
    appUI/DocumentEditor/viewer/PageMetrics.cpp \
    appUI/DocumentEditor/viewer/PagesTextEdit.cpp \
    appUI/DocumentEditor/dialogs/dialog_table_add.cpp \
    journal/dialog_zapros.cpp \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal_zapros.cpp \
    models/model_dynamic_view_patient.cpp \
    models/model_visits_control_patient.cpp \
    models/model_sved_gosp_patient.cpp \
    models/model_ivalid_patient.cpp \
    work_info_patient/visitors/dialog_visiting_control.cpp \
    models/model_visiting_control_all.cpp \
    models/filters/model_filter_invalid.cpp \
    appUI/Browser/mainwindow_web_browser.cpp \
    appUI/DocumentEditor/dialogs/dialog_page_settings.cpp \
    work_info_patient/dynamic_view/dialog_edit_other_dynamic_view.cpp

HEADERS  += mainwindow.h \
    dialog_patient.h \
    dialog_settings_user.h \
    dialog_login.h \
    dialog_registratura.h \
    dialog_reports.h \
    dialog_add_dynamic_view.h \
    dialog_add_visits.h \
    dialog_preview_print.h \
    new_changes_thread.h \
    work_info_patient/diagnos/dialog_diagnos_patient.h \
    dialog_hospitalization.h \
    dialog_copy_files_to_server.h \
    dialog_invalids_patient.h \
    dialog_find_filters.h \
    add_invalid_class.h \
    logs_save.h \
    models/model_main_table.h \
    models/model_find_table.h \
    dialog_edit_print.h \
    ui/Print/dialog_print_preview.h \
    appUI/dialog_app.h \
    appUI/Browser/dialog_browser.h \
    network/broadcastlog.h \
    appUI/Chat/dialog_chat.h \
    work_info_patient/diagnos/dialog_add_diagnos_and_dop_diagnos.h \
    work_info_patient/suicide/dialog_suicide.h \
    work_info_patient/list_not_work/dialog_list_not_work.h \
    work_info_patient/preparate/dialog_preparate.h \
    work_info_patient/day_stationar/dialog_day_stationar.h \
    work_info_patient/ood/dialog_ood.h \
    Objects/User/objects_app.h \
    journal/dialog_rvk.h \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal.h \
    Other_dialog_windows/dialog_fast_find_patient_id.h \
    Other_dialog_windows/dialog_diagnos_selected_patient.h \
    models/model_dynamic_view.h \
    models/model_diagnos_filter.h \
    appUI/DocumentEditor/window_document_editor.h \
    appUI/DocumentEditor/viewer/PageMetrics.h \
    appUI/DocumentEditor/viewer/PagesTextEdit.h \
    appUI/DocumentEditor/dialogs/dialog_table_add.h \
    journal/dialog_zapros.h \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal_zapros.h \
    models/model_dynamic_view_patient.h \
    models/model_visits_control_patient.h \
    models/model_sved_gosp_patient.h \
    models/model_ivalid_patient.h \
    work_info_patient/visitors/dialog_visiting_control.h \
    models/model_visiting_control_all.h \
    models/filters/model_filter_invalid.h \
    appUI/Browser/mainwindow_web_browser.h \
    appUI/DocumentEditor/dialogs/dialog_page_settings.h \
    work_info_patient/dynamic_view/dialog_edit_other_dynamic_view.h

FORMS    += mainwindow.ui \
    dialog_patient.ui \
    dialog_settings_user.ui \
    dialog_login.ui \
    dialog_registratura.ui \
    dialog_reports.ui \
    dialog_add_dynamic_view.ui \
    dialog_add_visits.ui \
    dialog_preview_print.ui \
    work_info_patient/diagnos/dialog_diagnos_patient.ui \
    dialog_hospitalization.ui \
    dialog_copy_files_to_server.ui \
    dialog_invalids_patient.ui \
    dialog_find_filters.ui \
    dialog_edit_print.ui \
    ui/Print/dialog_print_preview.ui \
    appUI/dialog_app.ui \
    appUI/Browser/dialog_browser.ui \
    appUI/Chat/dialog_chat.ui \
    work_info_patient/diagnos/dialog_add_diagnos_and_dop_diagnos.ui \
    work_info_patient/suicide/dialog_suicide.ui \
    work_info_patient/list_not_work/dialog_list_not_work.ui \
    work_info_patient/preparate/dialog_preparate.ui \
    work_info_patient/day_stationar/dialog_day_stationar.ui \
    work_info_patient/ood/dialog_ood.ui \
    journal/dialog_rvk.ui \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal.ui \
    Other_dialog_windows/dialog_fast_find_patient_id.ui \
    Other_dialog_windows/dialog_diagnos_selected_patient.ui \
    appUI/DocumentEditor/window_document_editor.ui \
    appUI/DocumentEditor/dialogs/dialog_table_add.ui \
    journal/dialog_zapros.ui \
    journal/Dialog_add_and_edit/dialog_add_and_edit_journal_zapros.ui \
    work_info_patient/visitors/dialog_visiting_control.ui \
    appUI/TasksUI/window_palner.ui \
    appUI/Browser/mainwindow_web_browser.ui \
    appUI/DocumentEditor/dialogs/dialog_page_settings.ui \
    work_info_patient/dynamic_view/dialog_edit_other_dynamic_view.ui

RESOURCES += \
    images.qrc
win32{
LIBS += -L /plugin/sqldrivers
}
#INCLUDEPATH += C:/ProjectPSI/nonstoklibrary/poppler-0.40.0/qt5/src

