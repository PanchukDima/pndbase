#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dialog_login login_win;
    Objects_app obj;

    //use default
    ui->listView->hide();

    if(QDate::currentDate().dayOfYear()== 255)
    {

    }
    if(login_win.exec())
    {
        settings_user();
        load_settings_from_sql();
        if(obj.rights_user[0])
        {
            ui->tabWidget_main->show();
            ui->pushButton_hide->show();
        }
        else
        {
            ui->tabWidget_main->hide();
            ui->pushButton_hide->hide();
        }
        ui->tab_diagnos->setEnabled(obj.rights_user[22]);
        ui->tab_view->setEnabled(obj.rights_user[23]);
        ui->tab_hospitalization->setEnabled(obj.rights_user[24]);
        ui->tab_invalid->setEnabled(obj.rights_user[25]);
        ui->tab_medicaments->setEnabled(obj.rights_user[26]);
        ui->tab_additional->setEnabled(obj.rights_user[27]);
        ui->tab_history_patient->setEnabled(obj.rights_user[28]);

        // Задаем стандартные настроки кнопки сворачивания
        QIcon icon(":/icon/png/images/sortIndicatorDown.png");
        ui->pushButton_hide->setIcon(icon);
        state_hide = true;
        //...
        ui->pushButton_add_invalid->setEnabled(obj.rights_user[13]);

        timer_who_is_online = new QTimer(this);
        timer_who_is_online->start(5000);

        connect(timer_who_is_online,SIGNAL(timeout()),SLOT(I_Online()));
        udpSocket = new QUdpSocket();
        udpSocket->bind(7979, QUdpSocket::ShareAddress);
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(loadCommand()));

        //settings_user();
        //settings_ui();
        connect(ui->action_settings_program,SIGNAL(triggered()),SLOT(open_win_settings()));
    }
    else
    {
        exit(0);
    }

    QTimer * rights_update_timer = new QTimer(this);
    rights_update_timer->start(5000);
    connect(rights_update_timer,SIGNAL(timeout()),SLOT(rights_update()));



    connect(ui->action_report_1,SIGNAL(triggered()),SLOT(gen_report_1()));
    connect(ui->action_report_9,SIGNAL(triggered(bool)),SLOT(gen_report_9()));
    connect(ui->action_report_forma_10,SIGNAL(triggered(bool)),SLOT(gen_report_19()));
    connect(ui->action_report_24,SIGNAL(triggered(bool)),SLOT(gen_report_24()));
    connect(ui->action_filter,SIGNAL(triggered(bool)),SLOT(open_find_filter()));


    //connect(ui->tableView,SIGNAL(activated(QModelIndex)),SLOT(load_all_info()));

    //контекстное меню
    connect(ui->tableView_dynamic_view,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_dynamic_view(QPoint)));
    connect(ui->tableView_visits_control,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_visits_control(QPoint)));
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_main_table(QPoint)));
    connect(ui->tableView_ood,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_ood_table(QPoint)));
    connect(ui->tableWidget_diagnos_patient,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_diagnos_table(QPoint)));
    connect(ui->tableView_sved_gosp,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_hospitalization_table(QPoint)));
    connect(ui->tableWidget_invalid_psi,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_invalid_table(QPoint)));
    connect(ui->tableView_suicide,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_suicide_table(QPoint)));
    connect(ui->tableView_list_not_work,SIGNAL(customContextMenuRequested(QPoint)),SLOT(context_menu_list_not_work_table(QPoint)));
    //...
    //Кнопки в дополнительной информации
    //Кнопки добавления
    connect(ui->pushButton_add_diagnos,SIGNAL(clicked(bool)),SLOT(add_diagnos_patient()));
    connect(ui->pushButton_add_dynamic_view,SIGNAL(clicked(bool)),SLOT(add_dynamic_view()));
    connect(ui->pushButton_add_hosp,SIGNAL(clicked(bool)),SLOT(add_hospitalization()));
    connect(ui->pushButton_add_not_work_list,SIGNAL(clicked(bool)),SLOT(add_list_not_work()));
    //connect(ui->pushButton_add_day_station,SIGNAL(clicked(bool)),SLOT());
    //connect(ui->pushButto_add_preparate,SIGNAL(clicked(bool)),SLOT());
    connect(ui->pushButton_add_suicide,SIGNAL(clicked(bool)),SLOT(add_suicide()));
    connect(ui->pushButton_add_ood,SIGNAL(clicked(bool)),SLOT(add_ood()));
    connect(ui->pushButton_add_visits_control,SIGNAL(clicked(bool)),SLOT(add_visit()));
    connect(ui->pushButton_add_today_visits_control,SIGNAL(clicked(bool)),SLOT(add_today_visit()));
    connect(ui->pushButton_yesterday,SIGNAL(clicked(bool)),SLOT(add_yesterday_visits()));
    connect(ui->pushButton_day_before_yesterday,SIGNAL(clicked(bool)),SLOT(add_day_before_yesterday_visits()));
    //Кнопки редактирования
    connect(ui->pushButton_edit_diagnos,SIGNAL(clicked(bool)),SLOT(edit_diagnos_patient()));
    connect(ui->pushButton_edit_hosp,SIGNAL(clicked(bool)),SLOT(edit_hospitalization()));
    connect(ui->pushButton_edit_notwork,SIGNAL(clicked(bool)),SLOT(edit_list_not_work()));
    //connect(ui->pushButton_edit_day_station,SIGNAL(clicked(bool)),SLOT());
    connect(ui->pushButton_edit_ood,SIGNAL(clicked(bool)),SLOT(edit_ood()));
    connect(ui->pushButton_edit_suicide,SIGNAL(clicked(bool)),SLOT(edit_suicide()));
    //connect(ui->pushButton_edit_preparate,SIGNAL(clicked(bool)),SLOT());
    connect(ui->pushButton_edit_visits_control,SIGNAL(clicked(bool)),SLOT(edit_visit()));
    connect(ui->pushButton_edit_dynamic_view,SIGNAL(clicked(bool)),SLOT(edit_dynamic_view()));

    //Кнопки удаления
    connect(ui->pushButton_del_diagnos,SIGNAL(clicked(bool)),SLOT(del_diagnos_patient()));
    connect(ui->pushButton_close_dynamic_view,SIGNAL(clicked(bool)),SLOT(close_dynamic_view()));
    connect(ui->pushButton_del_hosp,SIGNAL(clicked(bool)),SLOT(del_hospitalization()));
    connect(ui->pushButton_del_notwork,SIGNAL(clicked(bool)),SLOT(del_list_not_work()));
    //connect(ui->pushButton_del_day_station,SIGNAL(clicked(bool)),SLOT());
    connect(ui->pushButton_del_ood,SIGNAL(clicked(bool)),SLOT(del_ood()));
    connect(ui->pushButton_del_suicide,SIGNAL(clicked(bool)),SLOT(del_suicide()));
    //connect(ui->pushButton_del_preparate,SIGNAL(clicked(bool)),SLOT());
    connect(ui->pushButton_del_dynamic_view,SIGNAL(clicked(bool)),SLOT(del_dynamic_view()));
    connect(ui->pushButton_del_visits_control,SIGNAL(clicked(bool)),SLOT(del_visit()));

    connect(ui->pushButton_add_history_patient,SIGNAL(clicked(bool)),SLOT(added_files_patient()));


    connect(ui->pushButton_add_invalid,SIGNAL(clicked(bool)),SLOT(add_invalid()));
    connect(ui->pushButton_hide,SIGNAL(clicked(bool)),SLOT(hide_other_info()));
    connect(ui->action_app_window,SIGNAL(triggered(bool)),SLOT(open_app()));




    connect(ui->action_journal_rvk,SIGNAL(triggered(bool)),SLOT(open_journal_rvk()));

    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),SLOT(added_files_patient()));

    connect(ui->action_visitors,SIGNAL(triggered(bool)),SLOT(view_visiting()));
    connect(ui->pushButton_visiting_today,SIGNAL(clicked(bool)),SLOT(show_visiting_today()));
    connect(ui->tabWidget_main,SIGNAL(currentChanged(int)),SLOT(load_all_info()));
    connect(ui->tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),SLOT(load_all_info()));


}


MainWindow::~MainWindow()
{

    delete ui;
}
void MainWindow::loadCommand()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString stringData = datagram.data();
        QStringList strList = stringData.split("{split}");
        qDebug()<<strList;
        switch (strList[0].toInt()) {
        case 0:
            if(session_id == strList[1])
            {
                MainWindow::close();
            }
            break;
        case 1:
            if(session_id == strList[1])
            {
                QMessageBox::warning(this,strList[2],strList[3]);
            }
            break;
        }

    }
}
void MainWindow::I_Online()
{
    QByteArray datagram = "1{split}"+session_id.toUtf8()+"{split}"+staff_name.toUtf8();
    QUdpSocket * udpSocketSender_service = new QUdpSocket();
    udpSocketSender_service->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 7980);
    udpSocketSender_service->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //Сохранение размеров виджетов
    //    QSettings *settings = new QSettings("UISave.ini",QSettings::IniFormat);
    //    settings->beginGroup("MainTable");
    //    settings->setValue("sizeH", ui->tableView->height());
    //    settings->endGroup();
    //    settings->beginGroup("Sved_hosp");
    //    settings->setValue("sizeH", ui->tableWidget_sved_gospital->height());
    //    settings->endGroup();
    //    settings->beginGroup("point_time_notwork");
    //    settings->setValue("sizeH", ui->tableWidget_point_time_notwork->height());
    //    settings->endGroup();

    QFile files_delete("session.ini");
    files_delete.remove();
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    query.exec("UPDATE test.session SET date_time_stop='"+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+"', state=0 WHERE id="+session_id);
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    event->accept();
    QByteArray datagram = "0{split}"+session_id.toUtf8();
    QUdpSocket * udpSocketSender_service = new QUdpSocket();
    udpSocketSender_service->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 7980);
    udpSocketSender_service->close();
    qDebug()<<"Close Application";

}
void MainWindow::thread_new_changes()
{


    //        QThread* thread = new QThread;
    //        new_changes_thread * changes_new = new new_changes_thread;
    //        changes_new->global_session_id = session_id;
    //        changes_new->moveToThread(thread);
    //        connect(thread,SIGNAL(started()),changes_new,SLOT(check_new_changes_blanks()));

    //        connect(changes_new, SIGNAL(finished()), thread, SLOT(quit()));
    //        connect(changes_new, SIGNAL(finished()), changes_new, SLOT(deleteLater()));
    //        connect(changes_new,SIGNAL(status(QString)),SLOT(changes_new(QString)));

    //        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    //        thread->start();
}


void MainWindow::changes_new(QString status)
{
    qDebug()<<status;
}

void MainWindow::load_main_table()
{
    qDebug()<<"MainWindow: Function: load_main_table";
    clear_main_table();
    settings_ui();
}
void MainWindow::settings_ui()
{
    //Создаем объекты
    Objects_app obj;
    qDebug()<<"MainWindow: Function: settings_ui";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    valid_info = new QLabel();

    ui->checkBox_see_arhiv->show();



    fff = true;

    //str_find_patient->setEnabled(obj.rights_user[0]);


    //Конпки добавления
    ui->pushButton_add_diagnos->setEnabled(obj.rights_user[1]);
    ui->pushButton_add_dynamic_view->setEnabled(obj.rights_user[4]);
    ui->pushButton_add_hosp->setEnabled(obj.rights_user[10]);
    ui->pushButton_add_not_work_list->setEnabled(obj.rights_user[38]);
    ui->pushButton_add_day_station->setEnabled(obj.rights_user[41]);
    ui->pushButton_add_preparate->setEnabled(obj.rights_user[15]);
    ui->pushButton_add_suicide->setEnabled(obj.rights_user[30]);
    ui->pushButton_add_ood->setEnabled(obj.rights_user[34]);
    ui->pushButton_add_visits_control->setEnabled(obj.rights_user[6]);
    ui->pushButton_add_today_visits_control->setEnabled(obj.rights_user[7]);
    ui->pushButton_yesterday->setEnabled(obj.rights_user[7]);
    ui->pushButton_day_before_yesterday->setEnabled(obj.rights_user[7]);
    //Кнопки редактирования
    ui->pushButton_edit_diagnos->setEnabled(obj.rights_user[2]);

    ui->pushButton_edit_hosp->setEnabled(obj.rights_user[11]);
    ui->pushButton_edit_notwork->setEnabled(obj.rights_user[39]);
    ui->pushButton_edit_day_station->setEnabled(obj.rights_user[42]);
    ui->pushButton_edit_preparate->setEnabled(obj.rights_user[16]);
    ui->pushButton_edit_suicide->setEnabled(obj.rights_user[31]);
    ui->pushButton_edit_ood->setEnabled(obj.rights_user[35]);
    ui->pushButton_edit_visits_control->setEnabled(obj.rights_user[8]);
    ui->pushButton_edit_dynamic_view->setEnabled(obj.rights_user[46]);

    //Кнопки удаления
    ui->pushButton_del_diagnos->setEnabled(obj.rights_user[3]);
    ui->pushButton_close_dynamic_view->setEnabled(obj.rights_user[5]);
    ui->pushButton_del_hosp->setEnabled(obj.rights_user[12]);
    ui->pushButton_del_notwork->setEnabled(obj.rights_user[40]);
    ui->pushButton_del_day_station->setEnabled(obj.rights_user[43]);
    ui->pushButton_del_preparate->setEnabled(obj.rights_user[17]);
    ui->pushButton_del_suicide->setEnabled(obj.rights_user[32]);
    ui->pushButton_del_ood->setEnabled(obj.rights_user[36]);
    ui->pushButton_del_dynamic_view->setEnabled(obj.rights_user[45]);
    ui->pushButton_del_visits_control->setEnabled(obj.rights_user[9]);






    QTableView * patientTable = ui->tableView; //Основная таблица


    QTableWidget * diagnos_table = ui->tableWidget_diagnos_patient; //Диагнозы пациентов

    QTableWidget * invalid_table = ui->tableWidget_invalid_psi;

    QTableWidgetItem * name_3_collumn_main = new QTableWidgetItem();
    QTableWidgetItem * name_4_collumn_main = new QTableWidgetItem();
    QTableWidgetItem * name_5_collumn_main = new QTableWidgetItem();
    //Итемы таблицы динамики наблюдения
    QTableWidgetItem * name_1_collumn_dynamic_view = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_dynamic_view = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_dynamic_view = new QTableWidgetItem();
    QTableWidgetItem * name_4_collumn_dynamic_view = new QTableWidgetItem();
    QTableWidgetItem * name_5_collumn_dynamic_view = new QTableWidgetItem();
    //Итемы таблицы контроль посешщений
    QTableWidgetItem * name_1_collumn_control_pos = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_control_pos = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_control_pos = new QTableWidgetItem();
    //Итемы таблицы диагнозов
    QTableWidgetItem * name_1_collumn_diagnos_table = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_diagnos_table = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_diagnos_table = new QTableWidgetItem();
    QTableWidgetItem * name_4_collumn_diagnos_table = new QTableWidgetItem();
    QTableWidgetItem * name_5_collumn_diagnos_table = new QTableWidgetItem();
    QTableWidgetItem * name_6_collumn_diagnos_table = new QTableWidgetItem();
    //Итемы таблицы госпитализация
    QTableWidgetItem * name_1_collumn_gospit = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_gospit = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_gospit = new QTableWidgetItem();
    QTableWidgetItem * name_4_collumn_gospit = new QTableWidgetItem();

    //Итемы таблицы нетрудоспособности
    QTableWidgetItem * name_1_collumn_list_not_work = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_list_not_work = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_list_not_work = new QTableWidgetItem();
    //Итемы таблицы инвалидности
    QTableWidgetItem * name_1_collumn_invalid = new QTableWidgetItem();
    QTableWidgetItem * name_2_collumn_invalid = new QTableWidgetItem();
    QTableWidgetItem * name_3_collumn_invalid = new QTableWidgetItem();
    QTableWidgetItem * name_4_collumn_invalid = new QTableWidgetItem();
    QTableWidgetItem * name_5_collumn_invalid = new QTableWidgetItem();
    QTableWidgetItem * name_6_collumn_invalid = new QTableWidgetItem();

    //Итемы таблицы суицид
    // QTableWidgetItem * name_1_collumn_suicid = new QTableWidgetItem();

    //Настраиваем объекты


    //Настройка таблиц Рястягивание заголовка
    //patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //Рястягивает все по ширине таблицы




    diagnos_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    diagnos_table->verticalHeader()->setDefaultSectionSize(70);





    invalid_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // suicid_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Количество столбцов



    diagnos_table->setColumnCount(6);
    //gospit_table->setColumnCount(5);

    invalid_table->setColumnCount(7);

    //suicid_table->setColumnCount(2);

    //Скрытие столбцов
    patientTable->hideColumn(0);
    patientTable->hideColumn(1);


    diagnos_table->hideColumn(0);
    //gospit_table->hideColumn(0);

    invalid_table->hideColumn(0);

    //suicid_table->hideColumn(0);


    //    patientTable->setColumnWidth(0,50);
    //    patientTable->setColumnWidth(3,30);
    //    patientTable->setColumnWidth(4,50);
    //    patientTable->setColumnWidth(5,50);
    //    patientTable->setColumnWidth(8,70);
    //    patientTable->setColumnWidth(9,70);
    //    name_2_collumn_main->setText("");


    QFont font_table_header;

    // Выставляем размер шрифта заголовков
    font_table_header.setPointSize(11);
    //текст заголовка таблицы вывода пациентов
    name_3_collumn_main->setText("ФИО");
    name_4_collumn_main->setText("Серия\n паспорта");
    name_5_collumn_main->setText("Номер\n паспорта");

    //текст заголовка таблицы динамика наблюдения
    name_1_collumn_dynamic_view->setText("Наблюдение\nоказывается с:");
    name_2_collumn_dynamic_view->setText("Вид\nнаблюдения:");
    name_3_collumn_dynamic_view->setText("Статус");
    name_4_collumn_dynamic_view->setText("Врач/Медсестра");
    name_5_collumn_dynamic_view->setText("Дата\nзакрытия");
    //текст заголовка таблицы контроль посещений
    name_1_collumn_control_pos->setText("Назначено:\n");
    name_2_collumn_control_pos->setText("Назначил:\n");
    name_3_collumn_control_pos->setText("Дата явки:\n");
    //текст заголовка таблицы диагнозы
    name_1_collumn_diagnos_table->setText("Статус");
    name_2_collumn_diagnos_table->setText("Код по МКБ-10");
    name_3_collumn_diagnos_table->setText("Наименование\nпо МКБ-10");
    name_4_collumn_diagnos_table->setText("Дата\nпостановки\nдиагноза");
    name_5_collumn_diagnos_table->setText("Код врача");
    name_6_collumn_diagnos_table->setText("ФИО врача");
    //текст заголовка таблицы госпитализация
    name_1_collumn_gospit->setText("Инициатор\nгоспитализации");
    name_2_collumn_gospit->setText("Дата\nПоступления");
    name_3_collumn_gospit->setText("Дата выбытия");
    name_4_collumn_gospit->setText("Добавил\nИзменил");
    //текст заголовка таблицы лист нетрудоспособности
    name_1_collumn_list_not_work->setText("Дата открытия\nбольничного листа");
    name_2_collumn_list_not_work->setText("Дата закрытия\nбольничного листа");
    name_3_collumn_list_not_work->setText("Число дней ВН");
    //текст заголовка таблицы инвалидность по псих. заболеваниям
    name_1_collumn_invalid->setText("Дата\nустановления\nили\nпересмотра");
    name_2_collumn_invalid->setText("Группа\nинвалидности");
    name_3_collumn_invalid->setText("Дата очередного\nпереосвидетельствования");
    name_4_collumn_invalid->setText("Признан\nтрудоспособным");
    name_5_collumn_invalid->setText("Получена\n в другом учреждении");
    name_6_collumn_invalid->setText("Инвалид детства");

    //текст заголовка таблицы суицид

    //name_1_collumn_suicid->setText("Дата совершения");

    //Создаем новый тип шрифта

    QFont font_size_text;
    font_size_text.setPointSize(font_size);
    // Изменяем размер шрифта в строке поиска


    // Выставляем размер шрифта заголовков "Главной таблицы"
    name_3_collumn_main->setFont(font_table_header);
    name_4_collumn_main->setFont(font_table_header);
    name_5_collumn_main->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "Динамика наблюдения"
    name_1_collumn_dynamic_view->setFont(font_table_header);
    name_2_collumn_dynamic_view->setFont(font_table_header);
    name_3_collumn_dynamic_view->setFont(font_table_header);
    name_4_collumn_dynamic_view->setFont(font_table_header);
    name_5_collumn_dynamic_view->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "Контроль посещений"
    name_1_collumn_control_pos->setFont(font_table_header);
    name_2_collumn_control_pos->setFont(font_table_header);
    name_3_collumn_control_pos->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "Диагноз"
    name_1_collumn_diagnos_table->setFont(font_table_header);
    name_2_collumn_diagnos_table->setFont(font_table_header);
    name_3_collumn_diagnos_table->setFont(font_table_header);
    name_4_collumn_diagnos_table->setFont(font_table_header);
    name_5_collumn_diagnos_table->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "Госпитализация"
    name_1_collumn_gospit->setFont(font_table_header);
    name_2_collumn_gospit->setFont(font_table_header);
    name_3_collumn_gospit->setFont(font_table_header);
    name_4_collumn_gospit->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "Лист нетрудоспособности"
    name_1_collumn_list_not_work->setFont(font_table_header);
    name_2_collumn_list_not_work->setFont(font_table_header);
    name_3_collumn_list_not_work->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "инвалидность по псих. заболеваниям"
    name_1_collumn_invalid->setFont(font_table_header);
    name_2_collumn_invalid->setFont(font_table_header);
    name_3_collumn_invalid->setFont(font_table_header);
    name_4_collumn_invalid->setFont(font_table_header);
    name_5_collumn_invalid->setFont(font_table_header);
    name_6_collumn_invalid->setFont(font_table_header);
    // Выставляем размер шрифта заголовков таблицы "суицид"


    //name_1_collumn_suicid->setFont(font_table_header);

    //Добавляем итемы на "Главную таблицу"
    //patientTable->setHorizontalHeaderItem(0,name_1_collumn_main);
    //patientTable->setHorizontalHeaderItem(1,name_2_collumn_main);
    //patientTable->setHorizontalHeaderItem(2,name_2_collumn_main);

    //Добавляем итемы на таблицу "Динамика наблюдения"


    //Добавляем итемы на таблицу "Контроль посещений"

    //Добавляем итемы на таблицу "Диагноз"
    diagnos_table->setHorizontalHeaderItem(1,name_1_collumn_diagnos_table);
    diagnos_table->setHorizontalHeaderItem(2,name_2_collumn_diagnos_table);
    diagnos_table->setHorizontalHeaderItem(3,name_3_collumn_diagnos_table);
    diagnos_table->setHorizontalHeaderItem(4,name_4_collumn_diagnos_table);
    diagnos_table->setHorizontalHeaderItem(5,name_5_collumn_diagnos_table);
    diagnos_table->setHorizontalHeaderItem(6,name_6_collumn_diagnos_table);

    //Добавляем итемы на таблицу "инвалидность по псих. заболеваниям"
    invalid_table->setHorizontalHeaderItem(1,name_1_collumn_invalid);
    invalid_table->setHorizontalHeaderItem(2,name_2_collumn_invalid);
    invalid_table->setHorizontalHeaderItem(3,name_3_collumn_invalid);
    invalid_table->setHorizontalHeaderItem(4,name_4_collumn_invalid);
    invalid_table->setHorizontalHeaderItem(5,name_5_collumn_invalid);
    invalid_table->setHorizontalHeaderItem(6,name_6_collumn_invalid);
    //Добавляем итемы на таблицу "суицид"




    //меняем title
    if(db.open())
    {
        qDebug()<<"Update: Title mainWindow";
        QSqlQuery query;
        Objects_app obj;
        query.exec("SELECT staff.fname, staff.name, staff.mname FROM test.staff WHERE staff.id = "+obj.staff_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            QString name_login;
            name_login.append(query.value(0).toString()).append(" ").append(query.value(1).toString()).append(" ").append(query.value(2).toString());
            this->setWindowTitle("Вы вошли как: "+name_login);
            staff_name = name_login;

        }
    }
    else
    {
        logs_save log;

        qDebug()<<"Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss");
    }
    //Сохраняем настройки сессии для дальнейшего использования
    QSettings *settings = new QSettings("session.ini",QSettings::IniFormat);
    settings->setValue("staff_name", staff_name);
    settings->setValue("staff_id", staff_id);


    
}
void MainWindow::settings_maintoolbar()
{
    QPushButton * add_patient = new QPushButton();
    QPushButton * edit_patient = new QPushButton();
    QPushButton * view_patient = new QPushButton();
    QPushButton * del_patient = new QPushButton();
    QPushButton * filter_find = new QPushButton();
    QPushButton * debility_button_find = new QPushButton();


    QFont font_text;
    font_text.setPointSize(font_size);

    str_find_patient = new QLineEdit();
    str_find_patient->setClearButtonEnabled(true);
    str_find_patient->setFont(font_text);

    QIcon icon_add(":/icon/png/images/add_icon.png");
    QIcon icon_edit(":/icon/png/images/edit_icon.png");
    QIcon icon_view(":/icon/png/images/eye.png");
    QIcon icon_del(":/icon/png/images/remove-user.png");
    QIcon icon_find(":/icon/png/images/magnifier.png");

    str_find_patient->setMaximumWidth(250);
    str_find_patient->setToolTip("Для поиска по карте \nвведите только номер,\n фамилия и имя должны быть разделены\n одним пробелом");
    valid_info->setText("<-Введите номер карты или Фамилию");

    add_patient->setText("Добавить");
    add_patient->setIcon(icon_add);

    edit_patient->setText("Редактировать");
    edit_patient->setIcon(icon_edit);

    view_patient->setText("Просмотр");
    view_patient->setIcon(icon_view);

    del_patient->setText("Удалить");
    del_patient->setIcon(icon_del);

    debility_button_find->setText("Найти");
    debility_button_find->setIcon(icon_find);

    filter_find->setText("Фильтр поиска");
    filter_find->setIcon(icon_find);

    ui->mainToolBar->addWidget(add_patient);
    ui->mainToolBar->addWidget(edit_patient);
    ui->mainToolBar->addWidget(view_patient);
    ui->mainToolBar->addWidget(del_patient);
    ui->mainToolBar->addWidget(str_find_patient);
    ui->mainToolBar->addWidget(debility_button_find);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(filter_find);

    connect(add_patient,SIGNAL(clicked()),SLOT(added_info_patient()));
    connect(edit_patient,SIGNAL(clicked()),SLOT(edit_info_patient()));
    connect(view_patient,SIGNAL(clicked(bool)),SLOT(view_info_patient()));
    connect(del_patient,SIGNAL(clicked()),SLOT(del_info_patient()));
    connect(str_find_patient,SIGNAL(returnPressed()),SLOT(find_patients()));
    connect(filter_find,SIGNAL(clicked(bool)),SLOT(open_find_filter()));
    connect(debility_button_find,SIGNAL(clicked(bool)),SLOT(find_patients()));

    add_patient->setEnabled(rights_user[18]);
    edit_patient->setEnabled(rights_user[19]);
    view_patient->setEnabled(true);// add rights
    del_patient->setEnabled(rights_user[20]);
    ui->action_visitors->setEnabled(rights_user[37]);

}

void MainWindow::hide_other_info()
{
    qDebug()<<"MainWindow: Function: hide_other_info";
    if(state_hide)
    {
        ui->tabWidget_main->hide();
        QIcon icon(":/icon/png/images/sortIndicatorUp.png");
        ui->pushButton_hide->setIcon(icon);
        state_hide = false;
    }
    else
    {
        ui->tabWidget_main->show();
        QIcon icon(":/icon/png/images/sortIndicatorDown.png");
        ui->pushButton_hide->setIcon(icon);
        state_hide = true;
    }
}
void MainWindow::open_find_filter()
{
    qDebug()<<"MainWindow: Function: open_find_filter";
    Dialog_find_filters * dialog = new Dialog_find_filters();
    connect(dialog,SIGNAL(diagnos(QString,QString, int)),SLOT(select_row_on_table_to_medcard(QString,QString,int)));
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void MainWindow::open_win_settings()
{
    qDebug()<<"MainWindow: Function: open_win_settings";
    Dialog_settings_user settings_dialog;
    settings_dialog.exec();
    settings_user();
    if(str_find_patient->text()!="")
    {
        find_patients();
    }
}
void MainWindow::settings_user()
{
    qDebug()<<"MainWindow: Function: setting_user";
    font_size = 12;
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    ipdatabase = settings->value("ipdatabase").toString();
    font_size = settings->value("sizefonttexttable").toInt();
    QString path_files_patient = settings->value("path_files_patient").toString();
    filemodel = new QFileSystemModel();
    filemodel->setRootPath(path_files_patient);
    ui->listView->setModel(filemodel);
    ui->listView->setRootIndex(filemodel->setRootPath(path_files_patient));

    
}
void MainWindow::added_info_patient()
{
    qDebug()<<"MainWindow: Function: added_info_patient";
    Dialog_patient* dialog = new Dialog_patient(this);
    dialog->setParam(0,0,false);
    dialog->show();
    dialog->raise();
    dialog->activateWindow();


}
void MainWindow::edit_info_patient()
{
    qDebug()<<"MainWindow: Function: edit_info_patient";
    Dialog_patient * dialog = new Dialog_patient();
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        int id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toInt();
        dialog->setParam(1,id,rights_user[29]);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
        connect(dialog,SIGNAL(accepted()),model,SLOT(select()));
    }
    else
    {
        QMessageBox::warning(this,"Ошибка", "Выберете одного пациента");

    }
}
void MainWindow::view_info_patient()
{
    qDebug()<<"MainWindow: Function: view_info_patient";
    Dialog_patient * dialog = new Dialog_patient(this);
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        int id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toInt();
        dialog->setParam(2,id,rights_user[29]);
        dialog->show();
        dialog->raise();
        dialog->activateWindow();
    }
    else
    {

        QMessageBox::warning(this,"Ошибка", "Выберите одного пациента");
    }


}
void MainWindow::del_info_patient()
{
    qDebug()<<"MainWindow: Function: del_info_paient";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,1).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление Пациента"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {
            query.exec("UPDATE test.patient  SET delete_row='true' WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            find_patients();
        }

    }
    else
    {
        QMessageBox::warning(this,"Ошибка", "Выберете одного пациента");

    }
}
void MainWindow::show_document_patient()
{
    qDebug()<<"MainWindow: Function: show_document_patient";
    Dialog_patient dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        int id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toInt();
        dialog.setParam(3,id,rights_user[29]);
        if(dialog.exec())
        {
            find_patients();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка", "Выберете одного пациента");

    }
}

void MainWindow::clear_main_table()
{
    //    int c =0;
    //    //ui->tableWidget->clear();
    //    ui->mainToolBar->clear();
    //    for (c = ui->tableWidget->rowCount()-1; c >= 0; c--)
    //    {
    //        ui->tableWidget->removeRow(c);
    //    }

}


void MainWindow::clear_diagnos_table()
{
    qDebug()<<"MainWindow: Function: clear_diagnos_table";
    int c;
    int iColumns = ui->tableWidget_diagnos_patient->columnCount();
    int iRows = ui->tableWidget_diagnos_patient->rowCount();
    QList<QTableWidgetItem*> myList;

    for(int i = 0; i < iRows; ++i)
    {
        for(int j = 0; j < iColumns; ++j)
        {
            QTableWidgetItem* pWidget = ui->tableWidget_diagnos_patient->item(i, j);
            myList.append(pWidget);
            pWidget = 0;
        }
    }
    QListIterator<QTableWidgetItem*> i(myList);
    while(i.hasNext())
    {
        delete i.next();
    }
    //ui->mainToolBar->clear();
    for (c = ui->tableWidget_diagnos_patient->rowCount()-1; c >= 0; c--)
    {
        ui->tableWidget_diagnos_patient->removeRow(c);
    }
}

void MainWindow::clear_invalid_table()
{
    qDebug()<<"MainWindow: Function: clear_invalid_table";
    int iColumns = ui->tableWidget_invalid_psi->columnCount();
    int iRows = ui->tableWidget_invalid_psi->rowCount();
    QList<QTableWidgetItem*> myList;

    for(int i = 0; i < iRows; ++i)
    {
        for(int j = 0; j < iColumns; ++j)
        {
            QTableWidgetItem* pWidget = ui->tableWidget_invalid_psi->item(i, j);
            myList.append(pWidget);
            pWidget = 0;
        }
    }
    QListIterator<QTableWidgetItem*> i(myList);
    while(i.hasNext())
    {
        delete i.next();
    }
    int c;
    //    ui->tableWidget_invalid_psi->clear();
    //    ui->tableWidget_invalid_psi->clearContents();
    //    ui->mainToolBar->clear();
    for (c = ui->tableWidget_invalid_psi->rowCount()-1; c >= 0; c--)
    {
        ui->tableWidget_invalid_psi->removeRow(c);
    }
}


void MainWindow::find_patients()
{
    qDebug()<<"MainWindow: Function: find_patients";

    QString string_find = str_find_patient->text();
    qDebug()<<string_find;
    QStringList list_string_find = string_find.split(" ");
    if(string_find != "")
    {
        for(int i = 0;list_string_find.count()>i;i++)
        {
            list_string_find[i].replace(" ","");
            QString string = list_string_find[i];
            string[0] = string[0].toUpper();
            list_string_find[i] = string;
        }
    }
    string_find = list_string_find.join(" ");

    if(string_find.toInt()==0)
    {
        if(ui->checkBox_see_arhiv->isChecked())
        {
            model->setFilter(" concat LIKE '" + string_find + "%' AND area_id in("+area_append_user+")");
        }
        else
        {
            model->setFilter(" concat LIKE '" + string_find + "%' AND area_id in("+area_append_user+") AND status in(99,100,101)");
        }
    }
    else
    {
        if(ui->checkBox_see_arhiv->isChecked())
        {
            model->setFilter(" patients_find.id = " + string_find + " AND area_id in("+area_append_user+")");
        }
        else
        {
            model->setFilter(" patients_find.id = " + string_find + " AND area_id in("+area_append_user+") AND status in(99,100,101)");
        }
    }
    model->select();
    ui->tableView->resizeColumnsToContents();
    if(model->lastError().isValid())
    {
        qDebug()<<model->lastError();
    }


    //QFont font_text;
    //font_text.setPointSize(font_size);
    //ui->tableView->setFont(font_text);

}

void MainWindow::gen_report_1()
{
    qDebug()<<"MainWindow: Function: gen_report_1";
    Dialog_reports dialog;
    dialog.gen_report(1);
    dialog.exec();
}
void MainWindow::gen_report_9()
{
    qDebug()<<"MainWindow: Function: gen_report_9";
    Dialog_reports dialog;
    dialog.gen_report(9);
    dialog.exec();
}

void MainWindow::gen_report_19()
{
    qDebug()<<"MainWindow: Function: gen_report_19";
    Dialog_reports dialog;
    dialog.gen_report(19);
    dialog.exec();
}
void MainWindow::gen_report_24()
{
    qDebug()<<"MainWindow: Function: gen_report_19";
    Dialog_reports dialog;
    dialog.gen_report(24);
    dialog.exec();
}



void MainWindow::load_all_info()
{
    //очистка таблиц от старых данных
    qDebug()<<"MainWindow: Function: load_all_info";
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    QString path_files_patient = settings->value("path_files_patient").toString();

    clear_diagnos_table();

    clear_invalid_table();

    settings_ui();

    QSqlDatabase db = QSqlDatabase::database();
    int selected_tables = ui->tableView->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        ui->statusBar->showMessage(id);
    }
//        selected_id = id;
//        //load_other_info();
//        //Обновление моделей
//        //set_table_param();
//        list_not_work_model->setFilter("medcard_id ="+id);
//        list_not_work_model->select();
//        model_dynamic_view->setFilter("medcard_id ="+id);
//        model_dynamic_view->select();
//        model_visits_control->setFilter("medcard_id ="+id);
//        model_visits_control->select();
//        model_sved_gosp->setFilter("medcard_id ="+id);
//        model_sved_gosp->select();



//        //files model
//        //        if(QDir(path_files_patient+"/"+id).exists())
//        //        {
//        //            ui->listView->setRootIndex(filemodel->setRootPath(path_files_patient+"/"+id));
//        //        }
//        //        else
//        //        {
//        //            QDir(path_files_patient).mkdir(id);
//        //        }
//        if(db.open())
//        {

//            QSqlQuery query;
//            query.exec("SELECT diagnos_patient.id, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NULL AND diagnos_patient.medcard_id = "+id+"AND delete_row = false");
//            if(query.lastError().isValid())
//            {
//                qDebug()<<query.lastError();
//                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
//            }
//            int last_row_diagnos = ui->tableWidget_diagnos_patient->rowCount();
//            QList <QStringList> all_parent;
//            while (query.next()) {
//                QString id_value = query.value(0).toString();
//                QString diagnos_name_value = query.value(1).toString();
//                QString diagnos_code_value = query.value(2).toString();
//                QString fixing_diagnos_date_value = query.value(3).toDate().toString("dd.MM.yyyy");
//                QString doctor_fio_value;
//                doctor_fio_value.append(query.value(4).toString()).append("\n").append(query.value(5).toString()).append("\n").append(query.value(6).toString());
//                QString doctor_code_value = query.value(7).toString();
//                QStringList parent;
//                parent.append(id_value);
//                parent.append(diagnos_name_value);
//                parent.append(diagnos_code_value);
//                parent.append(fixing_diagnos_date_value);
//                parent.append(doctor_fio_value);
//                parent.append(doctor_code_value);

//                all_parent.append(parent);
//            }
//            Sleep(300);

//            query.exec("SELECT diagnos_patient.id, diagnos_patient.id_parent, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NOT NULL AND diagnos_patient.medcard_id = "+id+" AND delete_row='false'");
//            if(query.lastError().isValid())
//            {
//                qDebug()<<query.lastError();
//                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
//            }
//            QList <QStringList> all_child;

//            while (query.next()) {
//                QString id_value = query.value(0).toString();
//                QString id_parent_value = query.value(1).toString();
//                QString diagnos_name_value = query.value(2).toString();
//                QString diagnos_code_value = query.value(3).toString();
//                QString fixing_diagnos_date_value = query.value(4).toDate().toString("dd.MM.yyyy");
//                QString doctor_fio_value;
//                doctor_fio_value.append(query.value(5).toString()).append("\n").append(query.value(6).toString()).append("\n").append(query.value(7).toString());
//                QString doctor_code_value = query.value(8).toString();
//                QStringList child;
//                child.append(id_value);
//                child.append(id_parent_value);
//                child.append(diagnos_name_value);
//                child.append(diagnos_code_value);
//                child.append(fixing_diagnos_date_value);
//                child.append(doctor_fio_value);
//                child.append(doctor_code_value);
//                all_child.append(child);
//            }


//            QString parent_id;
//            int i = 0;
//            int row = 0;


//            while(i<all_parent.count())
//            {
//                QStringList out = all_parent[i];
//                parent_id = out[0];

//                QTableWidgetItem * id = new QTableWidgetItem();
//                QTableWidgetItem * state = new QTableWidgetItem();
//                QTableWidgetItem * diagnos_name = new QTableWidgetItem();
//                QTableWidgetItem * diagnos_code = new QTableWidgetItem();
//                QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
//                QTableWidgetItem * doctor_fio = new QTableWidgetItem();
//                QTableWidgetItem * doctor_code = new QTableWidgetItem();

//                id->setText(out[0]);
//                state->setText("Основной");
//                diagnos_name->setText(out[1]);
//                diagnos_code->setText(out[2]);
//                fixing_diagnos_date->setText(out[3]);
//                doctor_fio->setText(out[4]);
//                doctor_code->setText(out[5]);

//                QFont font_text;
//                font_text.setPointSize(font_size);

//                diagnos_name->setFont(font_text);
//                diagnos_code->setFont(font_text);
//                fixing_diagnos_date->setFont(font_text);
//                doctor_fio->setFont(font_text);
//                doctor_code->setFont(font_text);
//                state->setFont(font_text);

//                ui->tableWidget_diagnos_patient->insertRow(row);

//                ui->tableWidget_diagnos_patient->setItem(row,0,id);
//                ui->tableWidget_diagnos_patient->setItem(row,1,state);
//                ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
//                ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
//                ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
//                ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
//                ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);


//                row++;
//                i++;

//                for(int j=0;j<all_child.count(); j++)
//                {
//                    QStringList out = all_child[j];

//                    if(parent_id == out[1])
//                    {

//                        QTableWidgetItem * id = new QTableWidgetItem();
//                        QTableWidgetItem * state = new QTableWidgetItem();
//                        QTableWidgetItem * diagnos_name = new QTableWidgetItem();
//                        QTableWidgetItem * diagnos_code = new QTableWidgetItem();
//                        QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
//                        QTableWidgetItem * doctor_fio = new QTableWidgetItem();
//                        QTableWidgetItem * doctor_code = new QTableWidgetItem();

//                        id->setText(out[0]);
//                        state->setText("");
//                        diagnos_name->setText(out[2]);
//                        diagnos_code->setText(out[3]);
//                        fixing_diagnos_date->setText(out[4]);
//                        doctor_fio->setText(out[5]);
//                        doctor_code->setText(out[6]);

//                        QFont font_text;
//                        font_text.setPointSize(font_size);

//                        diagnos_name->setFont(font_text);
//                        diagnos_code->setFont(font_text);
//                        fixing_diagnos_date->setFont(font_text);
//                        doctor_fio->setFont(font_text);
//                        doctor_code->setFont(font_text);
//                        state->setFont(font_text);

//                        ui->tableWidget_diagnos_patient->insertRow(row);

//                        ui->tableWidget_diagnos_patient->setItem(row,0,id);
//                        ui->tableWidget_diagnos_patient->setItem(row,1,state);
//                        ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
//                        ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
//                        ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
//                        ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
//                        ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);
//                        row++;
//                    }


//                }

//            }
//            all_child.clear();
//            all_parent.clear();

//        }
//        else
//        {
//            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
//            logs_save log;
//            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
//        }
//        if(db.open())
//        {
//            QSqlQuery query;
//            query.exec("SELECT\
//                       hospitalization.id,\
//                       hospitalization.who_wrote,\
//                       hospitalization.date_up,\
//                       hospitalization.date_down,\
//                       staff.fname,\
//                       staff.name,\
//                       staff.mname\
//                       FROM \
//                       test.hospitalization,\
//                       test.staff\
//                       WHERE\
//                       hospitalization.staff_add_id = staff.id AND delete_row = '0' AND medcard_id="+id);
//                    if(query.lastError().isValid())
//            {
//                    qDebug()<<query.lastError();
//                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
//        }
//                    int last_row_hospitalization = ui->tableWidget_sved_gospital->rowCount();

//                    while (query.next())
//            {
//                QString id_value = query.value(0).toString();
//                int who_value = query.value(1).toInt();
//                QString who_table_value;
//                QString date_up_value = query.value(2).toDate().toString("dd.MM.yyyy");
//                QString date_down_value = query.value(3).toDate().toString("dd.MM.yyyy");
//                QString staff_fio_value;
//                staff_fio_value.append(query.value(4).toString()).append("\n").append(query.value(5).toString()).append("\n").append(query.value(6).toString());

//                switch (who_value) {
//                case 0:
//                    who_table_value = "СП";
//                    break;
//                case 1:
//                    who_table_value = "ПНД";
//                    break;
//                case 2:
//                    who_table_value = "Другое";
//                    break;
//                }
//                QTableWidgetItem * id = new QTableWidgetItem();
//                QTableWidgetItem * who = new QTableWidgetItem();
//                QTableWidgetItem * date_up = new QTableWidgetItem();
//                QTableWidgetItem * date_down = new QTableWidgetItem();
//                QTableWidgetItem * staff_fio = new QTableWidgetItem();

//                id->setText(id_value);
//                who->setText(who_table_value);
//                date_up->setText(date_up_value);
//                date_down->setText(date_down_value);
//                staff_fio->setText(staff_fio_value);

//                QFont font_text;
//                font_text.setPointSize(font_size);

//                id->setFont(font_text);
//                who->setFont(font_text);
//                date_up->setFont(font_text);
//                date_down->setFont(font_text);
//                staff_fio->setFont(font_text);

//                ui->tableWidget_sved_gospital->insertRow(last_row_hospitalization);

//                ui->tableWidget_sved_gospital->setItem(last_row_hospitalization,0,id);
//                ui->tableWidget_sved_gospital->setItem(last_row_hospitalization,1,who);
//                ui->tableWidget_sved_gospital->setItem(last_row_hospitalization,2,date_up);
//                ui->tableWidget_sved_gospital->setItem(last_row_hospitalization,3,date_down);
//                ui->tableWidget_sved_gospital->setItem(last_row_hospitalization,4,staff_fio);


//            }
//        }
//        else
//        {
//            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
//            logs_save log;
//            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
//        }
//        if(db.open())
//        {
//            int last_row_invalid = ui->tableWidget_invalid_psi->rowCount();
//            QSqlQuery query;
//            query.exec("SELECT id, group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs, inv_child FROM test.invalid_patient WHERE delete_row = 'false' AND medcard_id="+id);
//            if(query.lastError().isValid())
//            {
//                qDebug()<<query.lastError();
//                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
//            }
//            while(query.next())
//            {
//                QString id_value = query.value(0).toString();
//                QString group_inv_value;
//                QString work_yes_value;
//                QString from_other_value;
//                QString pereosved_value;
//                QString inv_child_value;
//                QString date_preview_value = query.value(4).toDate().toString("dd.MM.yyyy");
//                switch (query.value(1).toInt()) {
//                case -1:
//                    group_inv_value="";
//                    break;
//                case 0:
//                    group_inv_value="I";
//                    break;
//                case 1:
//                    group_inv_value="II";
//                    break;
//                case 2:
//                    group_inv_value="III";
//                    break;
//                }

//                if(query.value(2).toBool())
//                {
//                    work_yes_value ="Да";
//                }
//                else
//                {
//                    work_yes_value ="Нет";
//                }
//                if(query.value(3).toBool())
//                {
//                    from_other_value = "Да";
//                }
//                else
//                {
//                    from_other_value = "Нет";
//                }
//                if(query.value(6).toBool())
//                {
//                    pereosved_value = "Бессрочно";
//                }
//                else
//                {
//                    pereosved_value = query.value(5).toDate().toString("dd.MM.yyyy");
//                }
//                if(query.value(7).toBool())
//                {
//                    inv_child_value = "Да";
//                }
//                else
//                {
//                    inv_child_value = "Нет";
//                }
//                QTableWidgetItem * id = new QTableWidgetItem();
//                QTableWidgetItem * group_inv = new QTableWidgetItem();
//                QTableWidgetItem * work_yes = new QTableWidgetItem();
//                QTableWidgetItem * from_other = new QTableWidgetItem();
//                QTableWidgetItem * pereosved = new QTableWidgetItem();
//                QTableWidgetItem * date_preview = new QTableWidgetItem();
//                QTableWidgetItem * inv_child = new QTableWidgetItem();

//                id->setText(id_value);
//                group_inv->setText(group_inv_value);
//                work_yes->setText(work_yes_value);
//                from_other->setText(from_other_value);
//                pereosved->setText(pereosved_value);
//                date_preview->setText(date_preview_value);
//                inv_child->setText(inv_child_value);

//                QFont font_text;
//                font_text.setPointSize(font_size);

//                id->setFont(font_text);
//                group_inv->setFont(font_text);
//                work_yes->setFont(font_text);
//                from_other->setFont(font_text);
//                pereosved->setFont(font_text);
//                date_preview->setFont(font_text);
//                inv_child->setFont(font_text);

//                ui->tableWidget_invalid_psi->insertRow(last_row_invalid);

//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,0,id);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,1,date_preview);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,2,group_inv);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,3,pereosved);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,4,work_yes);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,5,from_other);
//                ui->tableWidget_invalid_psi->setItem(last_row_invalid,6,inv_child);

//            }
//        }
//        else
//        {
//            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
//            logs_save log;
//            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
//        }
//        if(db.open())
//        {
//            model_suicide->setFilter("delete_row = 'false' AND medcard_id = "+ id);
//            model_suicide->select();
//        }
//        if(db.open())
//        {
//            model_ood->setFilter("delete_row = 'false' AND medcard_id = "+ id);
//            model_ood->select();
//        }
//    }
    if(ui->tableView_dynamic_view->isVisible())
    {
        load_model_dynamic_view();
        qDebug()<<"dynamic_load";
    }
    if(ui->tableView_list_not_work->isVisible())
    {
        load_list_not_work_model();
        qDebug()<<"list_not_work_load";
    }
    if(ui->tableView_visits_control->isVisible())
    {
        load_model_visits_control();
        qDebug()<<"visits_control_load";
    }
//    if(ui->tableWidget_sved_gospital->isVisible())
//    {
//        load_model_sved_gosp();
//    }
    if(ui->tableWidget_diagnos_patient->isVisible())
    {
        load_diagnos_table();
        qDebug()<<"diagnos_table";
    }
    if(ui->tableView_sved_gosp->isVisible())
    {
        load_model_sved_gosp();
        qDebug()<<"hosp";
    }
    if(ui->tableWidget_invalid_psi->isVisible())
    {
        load_invalid_table();
        qDebug()<<"invalid";
    }
    if(ui->tableView_suicide->isVisible())
    {
        load_suicide_table();
        qDebug()<<"suicide";
    }
    if(ui->tableView_ood->isVisible())
    {
        load_ood_table();
        qDebug()<<"ood";
    }


}
void MainWindow::load_list_not_work_model()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        list_not_work_model->setFilter("medcard_id ="+id+" AND delete_row = 'false'");
        list_not_work_model->select();
    }
}

void MainWindow::load_model_dynamic_view()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        model_dynamic_view->setFilter("delete_row = false AND medcard_id ="+id);
        model_dynamic_view->setSort(0,Qt::DescendingOrder);
        model_dynamic_view->select();
    }
}

void MainWindow::load_model_visits_control()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        model_visits_control->setFilter("medcard_id ="+id);
        model_visits_control->select();
    }
}

void MainWindow::load_model_sved_gosp()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        model_sved_gosp->setFilter("medcard_id ="+id);
        model_sved_gosp->select();
        ui->tableView_sved_gosp->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
        ui->tableView_sved_gosp->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
        ui->tableView_sved_gosp->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
        ui->tableView_sved_gosp->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    }
}

void MainWindow::load_diagnos_table()
{

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    int selected_tables = ui->tableView->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        if(db.open())
        {

            QSqlQuery query;
            query.exec("SELECT diagnos_patient.id, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NULL AND diagnos_patient.medcard_id = "+id+" AND delete_row = false order by id ");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            int last_row_diagnos = ui->tableWidget_diagnos_patient->rowCount();
            QList <QStringList> all_parent;
            while (query.next()) {
                QString id_value = query.value(0).toString();
                QString diagnos_name_value = query.value(1).toString();
                QString diagnos_code_value = query.value(2).toString();
                QString fixing_diagnos_date_value = query.value(3).toDate().toString("dd.MM.yyyy");
                QString doctor_fio_value;
                doctor_fio_value.append(query.value(4).toString()).append("\n").append(query.value(5).toString()).append("\n").append(query.value(6).toString());
                QString doctor_code_value = query.value(7).toString();
                QStringList parent;
                parent.append(id_value);
                parent.append(diagnos_name_value);
                parent.append(diagnos_code_value);
                parent.append(fixing_diagnos_date_value);
                parent.append(doctor_fio_value);
                parent.append(doctor_code_value);

                all_parent.append(parent);
            }
#ifdef _WIN32
  Sleep(300);
#endif

            query.exec("SELECT diagnos_patient.id, diagnos_patient.id_parent, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NOT NULL AND diagnos_patient.medcard_id = "+id+" AND delete_row='false'");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            QList <QStringList> all_child;

            while (query.next()) {
                QString id_value = query.value(0).toString();
                QString id_parent_value = query.value(1).toString();
                QString diagnos_name_value = query.value(2).toString();
                QString diagnos_code_value = query.value(3).toString();
                QString fixing_diagnos_date_value = query.value(4).toDate().toString("dd.MM.yyyy");
                QString doctor_fio_value;
                doctor_fio_value.append(query.value(5).toString()).append("\n").append(query.value(6).toString()).append("\n").append(query.value(7).toString());
                QString doctor_code_value = query.value(8).toString();
                QStringList child;
                child.append(id_value);
                child.append(id_parent_value);
                child.append(diagnos_name_value);
                child.append(diagnos_code_value);
                child.append(fixing_diagnos_date_value);
                child.append(doctor_fio_value);
                child.append(doctor_code_value);
                all_child.append(child);
            }


            QString parent_id;
            int i = 0;
            int row = 0;


            while(i<all_parent.count())
            {
                QStringList out = all_parent[i];
                parent_id = out[0];

                QTableWidgetItem * id = new QTableWidgetItem();
                QTableWidgetItem * state = new QTableWidgetItem();
                QTableWidgetItem * diagnos_name = new QTableWidgetItem();
                QTableWidgetItem * diagnos_code = new QTableWidgetItem();
                QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
                QTableWidgetItem * doctor_fio = new QTableWidgetItem();
                QTableWidgetItem * doctor_code = new QTableWidgetItem();

                id->setText(out[0]);
                state->setText("Основной");
                diagnos_name->setText(out[1]);
                diagnos_code->setText(out[2]);
                fixing_diagnos_date->setText(out[3]);
                doctor_fio->setText(out[4]);
                doctor_code->setText(out[5]);

                QFont font_text;
                font_text.setPointSize(font_size);

                diagnos_name->setFont(font_text);
                diagnos_code->setFont(font_text);
                fixing_diagnos_date->setFont(font_text);
                doctor_fio->setFont(font_text);
                doctor_code->setFont(font_text);
                state->setFont(font_text);

                ui->tableWidget_diagnos_patient->insertRow(row);

                ui->tableWidget_diagnos_patient->setItem(row,0,id);
                ui->tableWidget_diagnos_patient->setItem(row,1,state);
                ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
                ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
                ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
                ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
                ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);


                row++;
                i++;

                for(int j=0;j<all_child.count(); j++)
                {
                    QStringList out = all_child[j];

                    if(parent_id == out[1])
                    {

                        QTableWidgetItem * id = new QTableWidgetItem();
                        QTableWidgetItem * state = new QTableWidgetItem();
                        QTableWidgetItem * diagnos_name = new QTableWidgetItem();
                        QTableWidgetItem * diagnos_code = new QTableWidgetItem();
                        QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
                        QTableWidgetItem * doctor_fio = new QTableWidgetItem();
                        QTableWidgetItem * doctor_code = new QTableWidgetItem();

                        id->setText(out[0]);
                        state->setText("");
                        diagnos_name->setText(out[2]);
                        diagnos_code->setText(out[3]);
                        fixing_diagnos_date->setText(out[4]);
                        doctor_fio->setText(out[5]);
                        doctor_code->setText(out[6]);

                        QFont font_text;
                        font_text.setPointSize(font_size);

                        diagnos_name->setFont(font_text);
                        diagnos_code->setFont(font_text);
                        fixing_diagnos_date->setFont(font_text);
                        doctor_fio->setFont(font_text);
                        doctor_code->setFont(font_text);
                        state->setFont(font_text);

                        ui->tableWidget_diagnos_patient->insertRow(row);

                        ui->tableWidget_diagnos_patient->setItem(row,0,id);
                        ui->tableWidget_diagnos_patient->setItem(row,1,state);
                        ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
                        ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
                        ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
                        ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
                        ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);
                        row++;
                    }


                }

            }
            all_child.clear();
            all_parent.clear();

        }
        else
        {
            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
            logs_save log;
            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
        }
    }
}



void MainWindow::load_invalid_table()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;

    int selected_tables = ui->tableView->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        if(db.open())
        {
            int last_row_invalid = ui->tableWidget_invalid_psi->rowCount();
            QSqlQuery query;
            query.exec("SELECT id, group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs, inv_child FROM test.invalid_patient WHERE delete_row = 'false' AND medcard_id="+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while(query.next())
            {
                QString id_value = query.value(0).toString();
                QString group_inv_value;
                QString work_yes_value;
                QString from_other_value;
                QString pereosved_value;
                QString inv_child_value;
                QString date_preview_value = query.value(4).toDate().toString("dd.MM.yyyy");
                switch (query.value(1).toInt()) {
                case -1:
                    group_inv_value="";
                    break;
                case 0:
                    group_inv_value="I";
                    break;
                case 1:
                    group_inv_value="II";
                    break;
                case 2:
                    group_inv_value="III";
                    break;
                }

                if(query.value(2).toBool())
                {
                    work_yes_value ="Да";
                }
                else
                {
                    work_yes_value ="Нет";
                }
                if(query.value(3).toBool())
                {
                    from_other_value = "Да";
                }
                else
                {
                    from_other_value = "Нет";
                }
                if(query.value(6).toBool())
                {
                    pereosved_value = "Бессрочно";
                }
                else
                {
                    pereosved_value = query.value(5).toDate().toString("dd.MM.yyyy");
                }
                if(query.value(7).toBool())
                {
                    inv_child_value = "Да";
                }
                else
                {
                    inv_child_value = "Нет";
                }
                QTableWidgetItem * id = new QTableWidgetItem();
                QTableWidgetItem * group_inv = new QTableWidgetItem();
                QTableWidgetItem * work_yes = new QTableWidgetItem();
                QTableWidgetItem * from_other = new QTableWidgetItem();
                QTableWidgetItem * pereosved = new QTableWidgetItem();
                QTableWidgetItem * date_preview = new QTableWidgetItem();
                QTableWidgetItem * inv_child = new QTableWidgetItem();

                id->setText(id_value);
                group_inv->setText(group_inv_value);
                work_yes->setText(work_yes_value);
                from_other->setText(from_other_value);
                pereosved->setText(pereosved_value);
                date_preview->setText(date_preview_value);
                inv_child->setText(inv_child_value);

                QFont font_text;
                font_text.setPointSize(font_size);

                id->setFont(font_text);
                group_inv->setFont(font_text);
                work_yes->setFont(font_text);
                from_other->setFont(font_text);
                pereosved->setFont(font_text);
                date_preview->setFont(font_text);
                inv_child->setFont(font_text);

                ui->tableWidget_invalid_psi->insertRow(last_row_invalid);

                ui->tableWidget_invalid_psi->setItem(last_row_invalid,0,id);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,1,date_preview);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,2,group_inv);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,3,pereosved);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,4,work_yes);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,5,from_other);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,6,inv_child);

            }
        }
        else
        {
            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
            logs_save log;
            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
        }
    }
}

void MainWindow::load_suicide_table()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        model_suicide->setFilter("delete_row = 'false' AND medcard_id = "+ id);
        model_suicide->select();
    }
}

void MainWindow::load_ood_table()
{
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        model_ood->setFilter("delete_row = 'false' AND medcard_id = "+ id);
        model_ood->select();
    }
}

void MainWindow::load_other_info()
{
    QString id = selected_id;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(ui->tableWidget_diagnos_patient->isVisible())
    {
        if(db.open())
        {


            query.exec("SELECT diagnos_patient.id, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NULL AND diagnos_patient.medcard_id = "+id+"AND delete_row = false");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            int last_row_diagnos = ui->tableWidget_diagnos_patient->rowCount();
            QList <QStringList> all_parent;
            while (query.next()) {
                QString id_value = query.value(0).toString();
                QString diagnos_name_value = query.value(1).toString();
                QString diagnos_code_value = query.value(2).toString();
                QString fixing_diagnos_date_value = query.value(3).toDate().toString("dd.MM.yyyy");
                QString doctor_fio_value;
                doctor_fio_value.append(query.value(4).toString()).append("\n").append(query.value(5).toString()).append("\n").append(query.value(6).toString());
                QString doctor_code_value = query.value(7).toString();
                QStringList parent;
                parent.append(id_value);
                parent.append(diagnos_name_value);
                parent.append(diagnos_code_value);
                parent.append(fixing_diagnos_date_value);
                parent.append(doctor_fio_value);
                parent.append(doctor_code_value);

                all_parent.append(parent);
            }
#ifdef _WIN32
  Sleep(300);
#endif

            query.exec("SELECT diagnos_patient.id, diagnos_patient.id_parent, diagnos.name, diagnos.code, diagnos_patient.fixing_diagnos_date, staff.fname, staff.name, staff.mname, staff.code FROM test.diagnos_patient, test.diagnos, test.staff WHERE diagnos_patient.diagnos_id = diagnos.id AND diagnos_patient.staff_add_id = staff.id AND diagnos_patient.id_parent IS NOT NULL AND diagnos_patient.medcard_id = "+id+" AND delete_row='false'");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            QList <QStringList> all_child;

            while (query.next()) {
                QString id_value = query.value(0).toString();
                QString id_parent_value = query.value(1).toString();
                QString diagnos_name_value = query.value(2).toString();
                QString diagnos_code_value = query.value(3).toString();
                QString fixing_diagnos_date_value = query.value(4).toDate().toString("dd.MM.yyyy");
                QString doctor_fio_value;
                doctor_fio_value.append(query.value(5).toString()).append("\n").append(query.value(6).toString()).append("\n").append(query.value(7).toString());
                QString doctor_code_value = query.value(8).toString();
                QStringList child;
                child.append(id_value);
                child.append(id_parent_value);
                child.append(diagnos_name_value);
                child.append(diagnos_code_value);
                child.append(fixing_diagnos_date_value);
                child.append(doctor_fio_value);
                child.append(doctor_code_value);
                all_child.append(child);
            }


            QString parent_id;
            int i = 0;
            int row = 0;


            while(i<all_parent.count())
            {
                QStringList out = all_parent[i];
                parent_id = out[0];

                QTableWidgetItem * id = new QTableWidgetItem();
                QTableWidgetItem * state = new QTableWidgetItem();
                QTableWidgetItem * diagnos_name = new QTableWidgetItem();
                QTableWidgetItem * diagnos_code = new QTableWidgetItem();
                QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
                QTableWidgetItem * doctor_fio = new QTableWidgetItem();
                QTableWidgetItem * doctor_code = new QTableWidgetItem();

                id->setText(out[0]);
                state->setText("Основной");
                diagnos_name->setText(out[1]);
                diagnos_code->setText(out[2]);
                fixing_diagnos_date->setText(out[3]);
                doctor_fio->setText(out[4]);
                doctor_code->setText(out[5]);

                QFont font_text;
                font_text.setPointSize(font_size);

                diagnos_name->setFont(font_text);
                diagnos_code->setFont(font_text);
                fixing_diagnos_date->setFont(font_text);
                doctor_fio->setFont(font_text);
                doctor_code->setFont(font_text);
                state->setFont(font_text);

                ui->tableWidget_diagnos_patient->insertRow(row);

                ui->tableWidget_diagnos_patient->setItem(row,0,id);
                ui->tableWidget_diagnos_patient->setItem(row,1,state);
                ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
                ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
                ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
                ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
                ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);


                row++;
                i++;

                for(int j=0;j<all_child.count(); j++)
                {
                    QStringList out = all_child[j];

                    if(parent_id == out[1])
                    {

                        QTableWidgetItem * id = new QTableWidgetItem();
                        QTableWidgetItem * state = new QTableWidgetItem();
                        QTableWidgetItem * diagnos_name = new QTableWidgetItem();
                        QTableWidgetItem * diagnos_code = new QTableWidgetItem();
                        QTableWidgetItem * fixing_diagnos_date = new QTableWidgetItem();
                        QTableWidgetItem * doctor_fio = new QTableWidgetItem();
                        QTableWidgetItem * doctor_code = new QTableWidgetItem();

                        id->setText(out[0]);
                        state->setText("");
                        diagnos_name->setText(out[2]);
                        diagnos_code->setText(out[3]);
                        fixing_diagnos_date->setText(out[4]);
                        doctor_fio->setText(out[5]);
                        doctor_code->setText(out[6]);

                        QFont font_text;
                        font_text.setPointSize(font_size);

                        diagnos_name->setFont(font_text);
                        diagnos_code->setFont(font_text);
                        fixing_diagnos_date->setFont(font_text);
                        doctor_fio->setFont(font_text);
                        doctor_code->setFont(font_text);
                        state->setFont(font_text);

                        ui->tableWidget_diagnos_patient->insertRow(row);

                        ui->tableWidget_diagnos_patient->setItem(row,0,id);
                        ui->tableWidget_diagnos_patient->setItem(row,1,state);
                        ui->tableWidget_diagnos_patient->setItem(row,2,diagnos_code);
                        ui->tableWidget_diagnos_patient->setItem(row,3,diagnos_name);
                        ui->tableWidget_diagnos_patient->setItem(row,4,fixing_diagnos_date);
                        ui->tableWidget_diagnos_patient->setItem(row,5,doctor_code);
                        ui->tableWidget_diagnos_patient->setItem(row,6,doctor_fio);
                        row++;
                    }


                }

            }

        }
        else
        {
            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
            logs_save log;
            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
        }
    }
    if(ui->tableView_dynamic_view->isVisible())
    {
        model_dynamic_view->setFilter("medcard_id ="+id);
        model_visits_control->setFilter("medcard_id ="+id);
    }
    if(ui->tableView_sved_gosp->isVisible())
    {
        model_sved_gosp->setFilter("medcard_id ="+id);
    }
    if(ui->tableWidget_invalid_psi->isVisible())
    {
        if(db.open())
        {
            int last_row_invalid = ui->tableWidget_invalid_psi->rowCount();
            QSqlQuery query;
            query.exec("SELECT id, group_inv, priznan_trudosp, from_other, date_peresmotra, srok_ocherednogo_pereosvidet, bs, inv_child FROM test.invalid_patient WHERE delete_row = 'false' AND medcard_id="+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while(query.next())
            {
                QString id_value = query.value(0).toString();
                QString group_inv_value;
                QString work_yes_value;
                QString from_other_value;
                QString pereosved_value;
                QString inv_child_value;
                QString date_preview_value = query.value(4).toDate().toString("dd.MM.yyyy");
                switch (query.value(1).toInt()) {
                case -1:
                    group_inv_value="";
                    break;
                case 0:
                    group_inv_value="I";
                    break;
                case 1:
                    group_inv_value="II";
                    break;
                case 2:
                    group_inv_value="III";
                    break;
                }

                if(query.value(2).toBool())
                {
                    work_yes_value ="Да";
                }
                else
                {
                    work_yes_value ="Нет";
                }
                if(query.value(3).toBool())
                {
                    from_other_value = "Да";
                }
                else
                {
                    from_other_value = "Нет";
                }
                if(query.value(6).toBool())
                {
                    pereosved_value = "Бессрочно";
                }
                else
                {
                    pereosved_value = query.value(5).toDate().toString("dd.MM.yyyy");
                }
                if(query.value(7).toBool())
                {
                    inv_child_value = "Да";
                }
                else
                {
                    inv_child_value = "Нет";
                }
                QTableWidgetItem * id = new QTableWidgetItem();
                QTableWidgetItem * group_inv = new QTableWidgetItem();
                QTableWidgetItem * work_yes = new QTableWidgetItem();
                QTableWidgetItem * from_other = new QTableWidgetItem();
                QTableWidgetItem * pereosved = new QTableWidgetItem();
                QTableWidgetItem * date_preview = new QTableWidgetItem();
                QTableWidgetItem * inv_child = new QTableWidgetItem();

                id->setText(id_value);
                group_inv->setText(group_inv_value);
                work_yes->setText(work_yes_value);
                from_other->setText(from_other_value);
                pereosved->setText(pereosved_value);
                date_preview->setText(date_preview_value);
                inv_child->setText(inv_child_value);

                QFont font_text;
                font_text.setPointSize(font_size);

                id->setFont(font_text);
                group_inv->setFont(font_text);
                work_yes->setFont(font_text);
                from_other->setFont(font_text);
                pereosved->setFont(font_text);
                date_preview->setFont(font_text);
                inv_child->setFont(font_text);

                ui->tableWidget_invalid_psi->insertRow(last_row_invalid);

                ui->tableWidget_invalid_psi->setItem(last_row_invalid,0,id);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,1,date_preview);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,2,group_inv);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,3,pereosved);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,4,work_yes);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,5,from_other);
                ui->tableWidget_invalid_psi->setItem(last_row_invalid,6,inv_child);

            }
        }
        else
        {
            QMessageBox::warning(this,"Ошибка","Потеряно соединение с базой данных");
            logs_save log;
            log.error_log("Database disconect "+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
        }
    }
    if(ui->tableWidget_preparat_patient->isVisible())
    {

    }

}

void MainWindow::context_menu_dynamic_view(QPoint pos) // Контекстное меню для Таблицы Динамика наблюдения
{
    qDebug()<<"MainWindow: Function: context_menu_dynamic_view";
    QMenu *menu = new QMenu;
    menu->addAction("Добавить", this, SLOT(add_dynamic_view()))->setEnabled(rights_user[4]);
    menu->addAction("Закрыть", this, SLOT(close_dynamic_view()))->setEnabled(rights_user[5]);
    menu->addAction("Удалить",this, SLOT(del_dynamic_view()))->setEnabled(rights_user[45]);
    menu->exec(ui->tableView_dynamic_view->mapToGlobal(pos));
}
void MainWindow::context_menu_visits_control(QPoint pos) //Контекстное меню для Таблицы Контроль посещений
{
    qDebug()<<"MainWindow: Function: context_menu_visits_control";
    QMenu *menu = new QMenu;
    QString date;
    bool set1=rights_user[6];
    bool set2=rights_user[7];
    bool set3=rights_user[8];
    bool set4=rights_user[9];
    int selected_tables = ui->tableView_visits_control->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        date = ui->tableView_visits_control->model()->index(row,4).data(Qt::DisplayRole).toString();

    }
    if(date=="")
    {
        set2=true;
    }
    else
    {
        set2=false;
    }
    menu->addAction("Добавить", this, SLOT(add_visit()))->setEnabled(set1);
    menu->addAction("Посещение (сегодня)", this, SLOT(add_today_visit()))->setEnabled(set2);
    menu->addAction("Посещение (вчера)",this,SLOT(add_yesterday_visits()))->setEnabled(set2);
    menu->addAction("Посещение (позавчера", this, SLOT(add_day_before_yesterday_visits()))->setEnabled(set2);
    menu->addAction("Изменить", this, SLOT(edit_visit()))->setEnabled(set3); // это можно использовать для прав->setEnabled(false);
    menu->addAction("Удалить", this, SLOT(del_visit()))->setEnabled(set4);
    menu->exec(ui->tableView_visits_control->mapToGlobal(pos));
}
void MainWindow::context_menu_list_not_work_table(QPoint pos)
{
    QMenu *menu = new QMenu;
    menu->addAction("Добавить",this,SLOT(add_list_not_work()));
    menu->addAction("Изменить",this,SLOT(edit_list_not_work()));
    menu->addAction("Удалить",this,SLOT(del_list_not_work()));
    menu->exec(ui->tableView_list_not_work->mapToGlobal(pos));

}

void MainWindow::context_menu_main_table(QPoint pos) //Контекстное меню главной таблицы
{    
    qDebug()<<"MainWindow: Function: context_menu_main_table";
    QMenu *menu = new QMenu;
    QMenu *submenu_journal = new QMenu("Журналы", menu);
    QMenu *sub_menu_rvk = new QMenu("РВК",submenu_journal);
    QMenu *sub_menu_query = new QMenu("Запросов",submenu_journal);
    //QMenu *submenu_status = new QMenu("Статус", menu);
    menu->addAction("Добавить", this, SLOT(added_info_patient()))->setEnabled(rights_user[18]);
    menu->addAction("Изменить", this, SLOT(edit_info_patient()))->setEnabled(rights_user[19]);
    menu->addAction("Удалить", this, SLOT(del_info_patient()))->setEnabled(rights_user[20]);
    //menu->addMenu(submenu_status);
    //submenu_status->addAction("В Архив", this, SLOT(set_status_arhive()));
    menu->addSeparator();
    menu->addAction("Печать медкарты", this ,SLOT(print_medcard()));
    menu->addMenu(submenu_journal);
    submenu_journal->addMenu(sub_menu_rvk);
    submenu_journal->addMenu(sub_menu_query);
    sub_menu_rvk->addAction("Добавить запись",this, SLOT(add_zapis_rvk()));
    sub_menu_rvk->addAction("Найти записи",this,SLOT(find_zapis_rvk()));
    sub_menu_query->addAction("Добавить запись", this, SLOT(add_zapis_query()));
    sub_menu_query->addAction("Найти записи",this, SLOT(find_zapis_query()));
    menu->addAction("Документы",this,SLOT(show_document_patient()));
    menu->exec(ui->tableView->mapToGlobal(pos));

}
void MainWindow::context_menu_files_list(QPoint)
{

}

void MainWindow::add_zapis_rvk()
{
    Dialog_add_and_edit_journal dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {

        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.input_signal(id);
        if(dialog.exec())
        {

        }
    }
}


void MainWindow::add_zapis_query()
{

}
void MainWindow::find_zapis_rvk()
{
    Dialog_RVK dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {

        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.input_signal(id);
        if(dialog.exec())
        {

        }
    }
}
void MainWindow::find_zapis_query()
{

}
void MainWindow::add_list_not_work()
{
    Dialog_list_not_work dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(0,id);
        if(dialog.exec())
        {
            list_not_work_model->select();
        }
    }

}
void MainWindow::edit_list_not_work()
{
    Dialog_list_not_work dialog;
    int selected_tables = ui->tableView_list_not_work->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_list_not_work->currentIndex().row();
        QString id = ui->tableView_list_not_work->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(1,id);
        if(dialog.exec())
        {
            list_not_work_model->select();
        }
    }
}
void MainWindow::del_list_not_work()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_list_not_work->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_list_not_work->currentIndex().row();
        QString id = ui->tableView_list_not_work->model()->index(row,0).data(Qt::DisplayRole).toString();
        if(db.open())
        {
            query.exec("UPDATE test.notwork_list SET delete_row = 'true', staff_del_id = "+obj.staff_id+" WHERE id ="+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удален лист нетрудоспособности № "+id+"')");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
        }
        list_not_work_model->select();
    }
}
void MainWindow::context_menu_diagnos_table(QPoint pos)
{
    qDebug()<<"MainWindow: Function: context_menu_diagnos_table";
    bool status;
    int selected_tables = ui->tableWidget_diagnos_patient->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_diagnos_patient->currentRow();
        QString state = ui->tableWidget_diagnos_patient->item(cu_row,1)->text();
        if(state == "")
        {
            status = false;
        }
        else
        {
            status = true;
        }
    }
    QMenu *menu = new QMenu;
    menu->addAction("Добавить", this, SLOT(add_diagnos_patient()))->setEnabled(rights_user[1]);
    menu->addAction("Добавить дополнительный диагноз", this, SLOT(add_child_diagnost_patient()))->setEnabled(status);
    menu->addAction("Изменить", this, SLOT(edit_diagnos_patient()))->setEnabled(rights_user[2]); // это можно использовать для прав->setEnabled(false);
    menu->addAction("Удалить", this, SLOT(del_diagnos_patient()))->setEnabled(rights_user[3]);
    menu->exec(ui->tableWidget_diagnos_patient->mapToGlobal(pos));
}
void MainWindow::context_menu_hospitalization_table(QPoint pos)
{
    qDebug()<<"MainWindow: Function: context_menu_hospitaliation_table";
    QMenu *menu = new QMenu;
    menu->addAction("Добавить", this, SLOT(add_hospitalization()))->setEnabled(rights_user[10]);
    menu->addAction("Изменить", this, SLOT(edit_hospitalization()))->setEnabled(rights_user[11]); // это можно использовать для прав->setEnabled(false);
    menu->addAction("Удалить", this, SLOT(del_hospitalization()))->setEnabled(rights_user[12]);
    menu->exec(ui->tableView_sved_gosp->mapToGlobal(pos));
}
void MainWindow::context_menu_invalid_table(QPoint pos)
{
    qDebug()<<"MainWindow: Function: context_menu_invalid_table";
    QMenu *menu = new QMenu;
    //menu->addAction("Добавить", this, SLOT(add_invalid()));
    //menu->addAction("Изменить", this, SLOT(edit_hospitalization())); // это можно использовать для прав->setEnabled(false);
    try{
        menu->addAction("Снять", this, SLOT(del_invalid()))->setEnabled(rights_user[14]);
        menu->addAction("Изменить", this, SLOT(edit_invalid()))->setEnabled(rights_user[33]);
    }
    catch(...)
    {
        QMessageBox::warning(this,"Ошибка","Были изменены права");
        QMainWindow::repaint();
    }
    menu->exec(ui->tableWidget_invalid_psi->mapToGlobal(pos));
}
void MainWindow::context_menu_suicide_table(QPoint pos)
{
    QMenu *menu = new QMenu;
    menu->addAction("Добавить", this, SLOT(add_suicide()))->setEnabled(rights_user[30]);
    menu->addAction("Изменить", this, SLOT(edit_suicide()))->setEnabled(rights_user[31]);
    menu->addAction("Удалить", this, SLOT(del_suicide()))->setEnabled(rights_user[32]);
    menu->exec(ui->tableView_suicide->mapToGlobal(pos));
}
void MainWindow::context_menu_ood_table(QPoint pos)
{
    QMenu *menu = new QMenu;
    menu->addAction("Добавить", this, SLOT(add_ood()))->setEnabled(rights_user[34]);
    menu->addAction("Изменить", this, SLOT(edit_ood()))->setEnabled(rights_user[35]);
    menu->addAction("Удалить", this, SLOT(del_ood()))->setEnabled(rights_user[36]);
    menu->exec(ui->tableView_ood->mapToGlobal(pos));
}

void MainWindow::add_visit()
{
    qDebug()<<"MainWindow: Function: add_visit";
    Dialog_add_visits dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {

        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить посещение нужно сначала выбрать пациента");
    }

}
void MainWindow::edit_visit()
{
    qDebug()<<"MainWindow: Function: edit_visit";
    Dialog_add_visits dialog;
    Objects_app obj;
    int selected_tables = ui->tableView_visits_control->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        QString id = ui->tableView_visits_control->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(1,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя изменить того чего нет)");
    }
}
void MainWindow::del_visit()
{
    qDebug()<<"MainWindow: Function: del_visit";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_visits_control->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        QString id = ui->tableView_visits_control->model()->index(row,0).data(Qt::DisplayRole).toString();
//        int ret = QMessageBox::warning(this, tr("Удаление посещения!"),
//                                       tr("Вы точно хотите удалить?"),
//                                       QMessageBox::Yes|QMessageBox::No);



//        if(ret==16384)
//        {
//            bool bOk;
//            QString str = QInputDialog::getText( 0,
//                                                 "Причина удаления",
//                                                 "Причина удаления:",
//                                                 QLineEdit::Normal,
//                                                 "",
//                                                 &bOk
//                                                );
//            if (!bOk) {
//                // Была нажата кнопка Cancel
//            }
//            else
//            {
//                if(str =="")
//                {
//                    del_visit();
//                }
                query.exec("UPDATE test.visits_control  SET delete_row='1' WHERE id= "+id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }

                else
                {
                    query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удалено посещение № "+id+"')");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }

            load_all_info();

    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
    }
}
void MainWindow::add_today_visit()
{
    qDebug()<<"MainWindow: Function: add_today_visit";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QDate *date_presence;
    QString date_string = date_presence->currentDate().toString("dd.MM.yyyy");
    QString date;
    int selected_tables = ui->tableView_visits_control->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        date = ui->tableView_visits_control->model()->index(row,4).data(Qt::DisplayRole).toString();

    }
    if(date!="")
    {
        QMessageBox::warning(this,"Ошибка", "Посещение уже закрыто");
        return;
    }
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        QString id = ui->tableView_visits_control->model()->index(row,0).data(Qt::DisplayRole).toString();
        query.exec("UPDATE test.visits_control  SET date_presence='"+date_string+"' WHERE id= "+id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        load_all_info();
    }
}
void MainWindow::add_yesterday_visits()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QDate date_presence = QDate::currentDate().addDays(-1);
    QString date_string = date_presence.toString("dd.MM.yyyy");
    QString date;
    int selected_tables = ui->tableView_visits_control->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        date = ui->tableView_visits_control->model()->index(row,4).data(Qt::DisplayRole).toString();

    }
    if(date!="")
    {
        QMessageBox::warning(this,"Ошибка", "Посещение уже закрыто");
        return;
    }
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        QString id = ui->tableView_visits_control->model()->index(row,0).data(Qt::DisplayRole).toString();
        query.exec("UPDATE test.visits_control  SET date_presence='"+date_string+"' WHERE id= "+id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Закрыто посещение (вчера) № "+id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        load_all_info();
    }
}
void MainWindow::add_day_before_yesterday_visits()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    QDate date_presence = QDate::currentDate().addDays(-2);
    QString date_string = date_presence.toString("dd.MM.yyyy");
    QString date;
    int selected_tables = ui->tableView_visits_control->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        date = ui->tableView_visits_control->model()->index(row,4).data(Qt::DisplayRole).toString();

    }
    if(date!="")
    {
        QMessageBox::warning(this,"Ошибка", "Посещение уже закрыто");
        return;
    }
    if (selected_tables >= 0)
    {
        int row = ui->tableView_visits_control->currentIndex().row();
        QString id = ui->tableView_visits_control->model()->index(row,0).data(Qt::DisplayRole).toString();
        query.exec("UPDATE test.visits_control  SET date_presence='"+date_string+"' WHERE id= "+id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Закрыто посещение (позавчера) № "+id+"')");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        load_all_info();
    }
}
void MainWindow::add_dynamic_view()
{
    qDebug()<<"MainWindow: Function: add_dynamic_view";
    Dialog_add_dynamic_view dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();

        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            model_dynamic_view->select();
            model_visits_control->select();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить динамику наблюдения нужно сначала выбрать пациента");
    }
}
void MainWindow::close_dynamic_view()
{
    qDebug()<<"MainWindow: Function: close_dynamic_view";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_dynamic_view->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView_dynamic_view->currentIndex().row();
        QString id = ui->tableView_dynamic_view->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Закрытие динамики наблюдения"),
                                       tr("Вы точно хотите закрыть?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

            query.exec("UPDATE test.dynamic_view  SET status='1', cause='проблема?', staff_add_id = '"+obj.staff_id+"', stop_date = '"+QDate::currentDate().toString("dd.MM.yyyy")+"'  WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Закрыта динамика наблюдения № "+id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            model_dynamic_view->select();
        }
        else
        {
            return ;
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя закрыть того чего нет)");
    }

}
void MainWindow::del_dynamic_view()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_dynamic_view->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView_dynamic_view->currentIndex().row();
        QString id = ui->tableView_dynamic_view->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление динамики наблюдения"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

            query.exec("UPDATE test.dynamic_view  SET delete_row='true', status = 'true', staff_del_id = '"+obj.staff_id+"'  WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удалена динамика наблюдения № "+id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            model_dynamic_view->select();
        }
        else
        {
            return ;
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя закрыть того чего нет)");
    }
}

void MainWindow::edit_dynamic_view()
{
    qDebug()<<"MainWindow: Function: edit_dynamic_view";
    Dialog_add_dynamic_view dialog;
    Dialog_edit_other_dynamic_view dialog_other;
    int selected_tables = ui->tableView_dynamic_view->currentIndex().row();

    if (selected_tables >= 0)
    {
        int row = ui->tableView_dynamic_view->currentIndex().row();
        QString id = ui->tableView_dynamic_view->model()->index(row,0).data(Qt::DisplayRole).toString();
        QString dynamic = ui->tableView_dynamic_view->model()->index(row,3).data(Qt::DisplayRole).toString();
        if(dynamic == "АДН" || dynamic == "АПЛ" || dynamic == "ЛП")
        {
            dialog_other.setParam(1,id);

            if(dialog_other.exec())
            {
                model_dynamic_view->select();
            }
        }
        else
        {
            dialog.setParam(1,id,staff_id);
            if(dialog.exec())
            {
                model_dynamic_view->select();
            }
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя изменить того чего нет)");
    }

}
void MainWindow::add_diagnos_patient()
{
    qDebug()<<"MainWindow: Function: add_diagnos_patient";
    Dialog_add_diagnos_and_dop_diagnos dialog;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(id,staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить диагноз нужно сначала выбрать пациента");
    }
}
void MainWindow::add_child_diagnost_patient()
{
    qDebug()<<"MainWindow: Function: add_child_diagnos_patient";
    Dialog_diagnos_patient dialog;
    int selected_tables = ui->tableWidget_diagnos_patient->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_diagnos_patient->currentRow();
        QString id = ui->tableWidget_diagnos_patient->item(cu_row,0)->text();
        dialog.setParam(2,id,staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить диагноз нужно сначала выбрать пациента");
    }
}

void MainWindow::edit_diagnos_patient()
{
    qDebug()<<"MainWindow: Function: edit_diagnos_patient";
    Dialog_diagnos_patient dialog;
    int selected_tables = ui->tableWidget_diagnos_patient->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_diagnos_patient->currentRow();
        QString id = ui->tableWidget_diagnos_patient->item(cu_row,0)->text();
        dialog.setParam(1,id,staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы изменить диагноз нужно сначала выбрать диагноз для изменения");
    }
}
void MainWindow::del_diagnos_patient()
{
    qDebug()<<"MainWindow: Function: del_daigos_patient";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int selected_tables = ui->tableWidget_diagnos_patient->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_diagnos_patient->currentRow();
        QString id = ui->tableWidget_diagnos_patient->item(cu_row,0)->text();
        int ret = QMessageBox::warning(this, tr("Удаление Диагноза!"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

            query.exec("UPDATE test.diagnos_patient SET delete_row='true' WHERE id= "+id+" OR id_parent = "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удален Диагноз № "+id+"')");
            }
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
    }
}
void MainWindow::add_hospitalization()
{
    qDebug()<<"MainWindow: Function: add_hospitalization";
    Dialog_hospitalization dialog;
    Dialog_add_diagnos_and_dop_diagnos dialog_diagnos;

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString diagnos_code;
    Objects_app obj;

    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();

        if(db.open())
        {
            query.exec("SELECT diagnos_patient.id, diagnos.code  FROM test.diagnos ,test.diagnos_patient WHERE diagnos.id = diagnos_patient.diagnos_id AND  diagnos_patient.id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+id+" AND delete_row = 'false' AND id_parent is NULL)");
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            while(query.next())
            {
                diagnos_code = query.value(1).toString();
            }
        }
        if(diagnos_code=="")
        {
            int ret = QMessageBox::warning(this, tr("Отсутствует диагноз пациента"),
                                           tr("Отсутствует диагноз пациента\n Открыть окно добавления диагноза"),
                                           QMessageBox::Yes|QMessageBox::No);

            if(ret==16384)
            {
                dialog_diagnos.setParam(id,obj.staff_id);
                if(!dialog_diagnos.exec())
                {
                    return;
                }
                else
                {
                    dialog.setParam(0,id,obj.staff_id);
                    if(dialog.exec())
                    {
                        load_all_info();
                    }
                }
            }
            else if(ret == 65536)
            {
                return;
            }
        }
        else
        {
            int ret = QMessageBox::warning(this, tr("Добавление госпитализации пациенту"),
                                           tr("В госпитализации будет добавлен диагноз "+diagnos_code.toUtf8()),
                                           QMessageBox::Yes|QMessageBox::No);
            if(ret==16384)
            {
                dialog.setParam(0,id,obj.staff_id);
                if(dialog.exec())
                {
                    load_all_info();
                }
            }
            else
            {
                int ret = QMessageBox::warning(this, tr("Добавить другой диагноз"),
                                               tr("Открыть окно добавления диагноза?"),
                                               QMessageBox::Yes|QMessageBox::No);
                if(ret==16384)
                {

                    dialog_diagnos.setParam(id,obj.staff_id);
                    if(!dialog_diagnos.exec())
                    {
                        return;
                    }
                    else
                    {
                        dialog.setParam(0,id,obj.staff_id);
                        if(dialog.exec())
                        {
                            load_all_info();
                        }
                    }
                }
                else
                {
                    return;
                }
            }
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить посещение нужно сначала выбрать пациента");
    }
}
void MainWindow::edit_hospitalization()
{
    qDebug()<<"MainWindow: Function: edit_hospitaliation";
    Dialog_hospitalization dialog;
    Objects_app obj;
    int selected_tables = ui->tableView_sved_gosp->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_sved_gosp->currentIndex().row();
        QString id = ui->tableView_sved_gosp->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(1,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить посещение нужно сначала выбрать пациента");
    }
}
void MainWindow::del_hospitalization()
{
    qDebug()<<"MainWindow: Function: del_hospitaliation";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_sved_gosp->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_sved_gosp->currentIndex().row();
        QString id = ui->tableView_sved_gosp->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление госпитализации!"),
                                       tr("Вы точно хотите удалить?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

            query.exec("UPDATE test.hospitalization SET delete_row='true' WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удалена госпитализация № "+id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
    }
}
void MainWindow::add_invalid()
{
    qDebug()<<"MainWindow: Function: add_invalid";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QString diagnos_code;
    QString medcard_id;
    Dialog_add_diagnos_and_dop_diagnos dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        medcard_id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
    }
    if(db.open())
    {
        query.exec("SELECT diagnos_patient.id, diagnos.code  FROM test.diagnos ,test.diagnos_patient WHERE diagnos.id = diagnos_patient.diagnos_id AND  diagnos_patient.id = (SELECT max(diagnos_patient.id) FROM test.diagnos_patient WHERE medcard_id = "+medcard_id+" AND delete_row = 'false' AND id_parent is NULL)");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            diagnos_code = query.value(1).toString();
        }
    }
    if(diagnos_code=="")
    {
        int ret = QMessageBox::warning(this, tr("Отсутствует диагноз пациента"),
                                       tr("Отсутствует диагноз пациента\n Открыть окно добавления диагноза"),
                                       QMessageBox::Yes|QMessageBox::No);
        if(ret==16384)
        {

            dialog.setParam(medcard_id,obj.staff_id);
            if(!dialog.exec())
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        int ret = QMessageBox::warning(this, tr("Добавление инвалидности пациенту"),
                                       tr("В инвалидности будет добавлен диагноз "+diagnos_code.toUtf8()),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

        }
        else
        {
            int ret = QMessageBox::warning(this, tr("Добавить другой диагноз"),
                                           tr("Открыть окно добавления диагноза?"),
                                           QMessageBox::Yes|QMessageBox::No);
            if(ret==16384)
            {

                dialog.setParam(medcard_id,obj.staff_id);
                if(!dialog.exec())
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }

    //add_invalid_class invalid;
    //connect(&invalid,SIGNAL(send_ok()),this,SLOT(add_invalid_ok()));

    //QString date_inst;
    //QString date_inst_test = validate_date(ui->lineEdit_date_inst->text());
    //QString date_pereosved;
    //QString date_pereosved_test;
    //QString from_other;
    //QString work_yes;
    //QString inv_child;
    //QString first_value;
    //QString group_inv = ui->comboBox_group_inv->currentData().toString();
    //QString work_place = ui->comboBox_work_place->currentData().toString();
    //QString step_lost = ui->lineEdit_stepen_lost->text();
    //QString dubl = ui->lineEdit_dubl->text();
    //QString other_recomm = ui->lineEdit_other_recom->text();


    //int selected_tables2 = ui->tableView->currentIndex().row();
    //if (selected_tables2 >= 0)
    //{
    //    int row = ui->tableView->currentIndex().row();
    //    medcard_id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();


    //    if(ui->checkBox_bs->isChecked())
    //    {
    //        date_pereosved = "NULL";
    //    }
    //    else
    //    {
    //        date_pereosved_test = validate_date(ui->lineEdit_date_pereosved->text());
    //        if(date_pereosved_test=="exit")
    //        {
    //            return;
    //        }
    //        else
    //        {
    //            date_pereosved=date_pereosved_test;
    //        }
    //    }

    //if(ui->checkBox_inv_child->isChecked())
    //{
    //    inv_child = "true";
    //}
    //else
    //{
    //    inv_child = "false";
    //}
    //if(date_inst_test=="exit")
    //{
    //    return;
    //}
    //else
    //{
    //    date_inst=date_inst_test;
    //}
    //if(ui->checkBox_from_other->isChecked())
    //{
    //    from_other = "true";
    //}
    //else
    //{
    //    from_other = "false";
    //}
    //if(ui->checkBox_work_yes->isChecked())
    //{
    //    work_yes = "true";
    //}
    //else
    //{
    //    work_yes = "false";
    //}
    //if(ui->checkBox_first->isChecked())
    //{
    //    first_value = "true";
    //}
    //else
    //{
    //    first_value = "false";
    //}

    //invalid.send_data(medcard_id,obj.staff_id,group_inv,from_other,work_yes,date_inst,date_pereosved,inv_child,ui->checkBox_bs->isChecked(),first_value,work_place,step_lost,other_recomm,dubl);

    //}
    //else
    //{

    //}


    Dialog_invalids_patient dialog_inv;

    int selected_tables_1 = ui->tableView->currentIndex().row();
    if (selected_tables_1 >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();

        dialog_inv.setParam(1,id,obj.staff_id);
        if(dialog_inv.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить инвалидность нужно сначала выбрать пациента");
    }
}

void MainWindow::edit_invalid()
{
    Dialog_invalids_patient dialog;
    Objects_app obj;
    int selected_tables = ui->tableWidget_invalid_psi->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_invalid_psi->currentRow();
        QString id = ui->tableWidget_invalid_psi->item(cu_row,0)->text();
        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы изменить инвалидность нужно сначала выбрать одну из записей");
    }
}



void MainWindow::del_invalid()
{
    qDebug()<<"MainWindow: Function: del_invalid";
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableWidget_invalid_psi->selectionModel()->selectedRows().count();
    if (selected_tables == 1)
    {
        int cu_row = ui->tableWidget_invalid_psi->currentRow();
        QString id = ui->tableWidget_invalid_psi->item(cu_row,0)->text();
        int ret = QMessageBox::warning(this, tr("Снятие инвалидности!"),
                                       tr("Вы точно хотите снять ивалидность?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {

            query.exec("UPDATE test.invalid_patient SET delete_row='true', staff_del_id = "+obj.staff_id+" WHERE id= "+id);
            if(query.lastError().isValid())
            {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            }
            else
            {
                query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Снята инвалидность № "+id+"')");
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
            }
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
    }
}

void MainWindow::add_suicide()
{
    Dialog_suicide dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить информацию о суициде нужно сначала выбрать пациента");
    }

}

void MainWindow::edit_suicide()
{
    Dialog_suicide dialog;
    Objects_app obj;
    int selected_tables = ui->tableView_suicide->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_suicide->currentIndex().row();
        QString id = ui->tableView_suicide->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(1,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы изменить информацию о суициде нужно сначала выбрать пациента");
    }
}
void MainWindow::del_suicide()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_suicide->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_suicide->currentIndex().row();
        QString id = ui->tableView_suicide->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление информации о суициде!"),
                                       tr("Вы точно хотите удалить информацию о суициде?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {
            if(db.open())
            {
                query.exec("UPDATE test.suicide SET delete_row='true' WHERE id= "+id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                else
                {
                    query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удалена информация о суициде № "+id+"')");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
                load_all_info();
            }
        }
        else
        {
            QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
        }
    }
}

void MainWindow::add_ood()
{
    Dialog_ood dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы добавить информацию о ООД нужно сначала выбрать пациента");
    }

}
void MainWindow::edit_ood()
{
    Dialog_ood dialog;
    Objects_app obj;
    int selected_tables = ui->tableView_ood->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_ood->currentIndex().row();
        QString id = ui->tableView_ood->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(1,id,obj.staff_id);
        if(dialog.exec())
        {
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Чтобы изменить информацию о ООД нужно сначала выбрать пациента");
    }
}
void MainWindow::del_ood()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    int selected_tables = ui->tableView_ood->currentIndex().row();
    if (selected_tables >= 0)
    {
        int row = ui->tableView_ood->currentIndex().row();
        QString id = ui->tableView_ood->model()->index(row,0).data(Qt::DisplayRole).toString();
        int ret = QMessageBox::warning(this, tr("Удаление информации о ООД!"),
                                       tr("Вы точно хотите удалить информацию о ООД?"),
                                       QMessageBox::Yes|QMessageBox::No);

        if(ret==16384)
        {
            if(db.open())
            {
                query.exec("UPDATE test.ood SET delete_row='true' WHERE id= "+id);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                    QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                }
                else
                {
                    query.exec("INSERT INTO test.logs(staff_add_id, date_add, text) VALUES ('"+obj.staff_id+"', '"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'Удалена информация о ООД № "+id+"')");
                    if(query.lastError().isValid())
                    {
                        qDebug()<<query.lastError();
                        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
                    }
                }
            }
            load_all_info();
        }
    }
    else
    {
        QMessageBox::warning(this,"Ошибка","Нельзя удалить того чего нет)");
    }
}

void MainWindow::print_medcard()
{
    qDebug()<<"MainWindow: Function: print_medcard";
    Dialog_preview_print dialog;
    Objects_app obj;
    int selected_tables = ui->tableView->currentIndex().row();;
    if (selected_tables >= 0)
    {
        int row = ui->tableView->currentIndex().row();
        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
        dialog.setParam(0,id,obj.staff_id);
        if(dialog.exec())
        {
            find_patients();
        }
    }
}
void MainWindow::set_status_arhive()
{

}

void MainWindow::added_files_patient()
{
    qDebug()<<"MainWindow: Function: added_files_patient";

    QModelIndexList indexList = ui->listView->selectionModel()->selectedIndexes();
    QListIterator<QModelIndex> i(indexList);
    while(i.hasNext())
    {
        QModelIndex index = i.next();

        QString file =  filemodel->fileInfo(index).fileName();

        QStringList files = file.split(".");
        if(files.count()>1)
        {
            QDesktopServices::openUrl(QUrl("file:"+filemodel->fileInfo(index).absoluteFilePath()));
        }
        else if(files.count()==1)
        {
            ui->listView->setRootIndex(filemodel->setRootPath(filemodel->fileInfo(index).absoluteFilePath()));
        }
        //QDesktopServices::openUrl(QUrl(filemodel->fileInfo(index).absoluteFilePath()));
    }
}
//    Dialog_copy_files_to_server dialog;
//    int selected_tables = ui->tableView->currentIndex().row();
//    if (selected_tables >= 0)
//    {
//        int row = ui->tableView->currentIndex().row();
//        QString id = ui->tableView->model()->index(row,0).data(Qt::DisplayRole).toString();
//        dialog.setParam(id,staff_id);
//        if(dialog.exec())
//        {
//            find_patients();
//        }
//    }



void MainWindow::test_curent_version()
{
    qDebug()<<"MainWindow: Function: test_current_version";
    QString default_title = MainWindow::windowTitle();
    QStringList title = default_title.split("/");
    QSettings *settings = new QSettings("settings_user.ini",QSettings::IniFormat);
    int version = settings->value("CurrentVersion").toInt();
    QString version_str = settings->value("CurrentVersion").toString();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QEventLoop loop;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply * reply = manager->get(QNetworkRequest(QUrl("http://dolo86.dockerclients.ru/updater/Pathes/Version.txt")));
    loop.exec();
    QString x = reply->readAll();
    if(version==x.toInt())
    {
        MainWindow::setWindowTitle(title[0]+"/Текущая версия программы: "+version_str);
    }
    else
    {
        MainWindow::setWindowTitle(title[0]+"/Появилась новая версия программы");
    }

}

void MainWindow::open_app()
{
    Dialog_app dialog;
    dialog.exec();
}
void MainWindow::open_journal_rvk()
{
    Dialog_RVK dialog;
    dialog.exec();
}

void MainWindow::notification_server(QString str)
{

}

void MainWindow::rights_update()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;

    rights_user.clear();
    if(db.open())
    {
        query.exec("SELECT \
                   rights.rights\
                   FROM \
                   test.rights, \
                   test.staff, \
                   test.users\
                   WHERE \
                   staff.id = users.staff_id AND\
                users.id = rights.user_id AND\
                staff.status = '0' AND\
                staff.id = "+obj.staff_id);
                if(query.lastError().isValid())
        {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
                while(query.next())
        {

                QString str = query.value(0).toString();

                for(int n = 0; str.count()>n;n++)
        {
            QString state = str.at(n);
            if(state=="0")
            {
                rights_user.append(false);
            }
            else if(state=="1")
            {
                rights_user.append(true);
            }
        }
    }
    obj.rights_user = rights_user;
}
if(db.open())
{
    QStringList area_list;
    area_append_user.clear();
    query.exec("SELECT area_id FROM test.servising_area where staff_id="+obj.staff_id);
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    while(query.next())
    {
        area_list.append(query.value(0).toString());
    }
    for (int i = 0;area_list.count()>i;i++)
    {
        area_append_user.append(area_list[i]);
        if(i!=area_list.count()-1)
        {
            area_append_user.append(",");
        }
    }
    obj.servising_area = area_list;
}
if(db.open())
{
    query.exec("SELECT state FROM test.session WHERE id = "+session_id);
    if(query.lastError().isValid())
    {
        qDebug()<<query.lastError();
        QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
    while(query.next())
    {
        if(query.value(0).toInt() == 2)
        {
            QMessageBox::warning(this,"Упс...","Кажется кто-то зашел под вашей учетной записью");
            //QMainWindow::close();
        }
    }
}
}
void MainWindow::view_visiting()
{
    Dialog_visiting_control * dialog = new Dialog_visiting_control(this);
    connect(dialog,SIGNAL(get_medcard_id(QString, QString, int)),SLOT(select_row_on_table_to_medcard(QString, QString, int)));
    dialog->show();
    dialog->raise();
    dialog->activateWindow();

}
void MainWindow::show_visiting_today()
{
    Dialog_visiting_control * dialog = new Dialog_visiting_control(this);
    connect(dialog,SIGNAL(get_medcard_id(QString, QString, int)),SLOT(select_row_on_table_to_medcard(QString, QString, int)));
    dialog->view_green();
    dialog->show();
    dialog->raise();
    dialog->activateWindow();

}
void MainWindow::select_row_on_table_to_medcard(QString medcard_id, QString id ,int type)
{
    QModelIndexList mList;
    switch (type) {
    case 0:
        str_find_patient->setText(medcard_id);
        find_patients();
        ui->tabWidget_main->setCurrentIndex(1);
        ui->tableView->selectRow(0);
        mList = model_visits_control->match(model_visits_control->index(0,0),Qt::DisplayRole,id);
        ui->tableView_visits_control->setFocus();
        ui->tableView_visits_control->selectRow(mList.at(0).row());
        break;
    case 1:
        str_find_patient->setText(medcard_id);
        find_patients();
        ui->tabWidget_main->setCurrentIndex(0);
        ui->tableView->selectRow(0);
        break;
    case 2:

        break;
    }

}

void MainWindow::set_table_param()
{
    QFont font_text;
    font_text.setPointSize(font_size);

    delete model;
    model = nullptr;
    model = new model_main_table();

    delete list_not_work_model;
    list_not_work_model = nullptr;
    list_not_work_model = new QSqlRelationalTableModel();

    delete model_dynamic_view;
    model_dynamic_view = nullptr;
    model_dynamic_view = new model_dynamic_view_patient();

    delete model_visits_control;
    model_visits_control = nullptr;
    model_visits_control = new model_visits_control_patient();

    QSortFilterProxyModel *filter_sved_gosp = new QSortFilterProxyModel();
    delete model_sved_gosp;
    model_sved_gosp = nullptr;
    model_sved_gosp = new model_sved_gosp_patient();
    filter_sved_gosp->setSourceModel(model_sved_gosp);

    delete model_invalid;
    model_invalid = nullptr;
    model_invalid = new model_ivalid_patient();

    delete model_suicide;
    model_suicide = nullptr;
    model_suicide = new QSqlRelationalTableModel();

    delete model_ood;
    model_ood = nullptr;
    model_ood = new QSqlRelationalTableModel();

    model->setTable("patients_find");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setHeaderData(2,Qt::Horizontal,tr("ФИО"));
    model->setHeaderData(3,Qt::Horizontal,tr("Серия\nПаспорта"));
    model->setHeaderData(4,Qt::Horizontal, tr("Номер\nПаспорта"));
    model->setHeaderData(5,Qt::Horizontal, tr("Дата\nРождения"));
    model->setHeaderData(7,Qt::Horizontal,tr("Участок"));
    model->setHeaderData(8,Qt::Horizontal,tr("Улица"));
    model->setHeaderData(9,Qt::Horizontal,tr("Дом"));
    model->setHeaderData(10,Qt::Horizontal,tr("Корпус"));
    model->setHeaderData(11,Qt::Horizontal,tr("Квартира"));
    model->setHeaderData(12,Qt::Horizontal,tr("Телефон"));
    model->setFilter("id = 0");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setFont(font_text);

    ui->tableView->hideColumn(6);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->setColumnWidth(2,120);


    list_not_work_model->setTable("test.not_work_list_view");

    list_not_work_model->setRelation(6, QSqlRelation("test.staff_view", "id", "staff_name"));
    list_not_work_model->setRelation(7, QSqlRelation("test.staff_view", "id", "staff_name"));

    list_not_work_model->setHeaderData(0,Qt::Horizontal,tr("id"));
    list_not_work_model->setHeaderData(1,Qt::Horizontal,tr("medcard_id"));
    list_not_work_model->setHeaderData(2,Qt::Horizontal,tr("Дата открытия"));
    list_not_work_model->setHeaderData(3,Qt::Horizontal,tr("Дата закрытия"));
    list_not_work_model->setHeaderData(4,Qt::Horizontal,tr("Количество дней"));
    list_not_work_model->setHeaderData(5,Qt::Horizontal,tr("Диагноз"));
    list_not_work_model->setHeaderData(6,Qt::Horizontal,tr("Добавил"));
    list_not_work_model->setHeaderData(7,Qt::Horizontal,tr("Изменил"));
    list_not_work_model->setHeaderData(8,Qt::Horizontal,tr("Удален"));

    list_not_work_model->setFilter("medcard_id = 0 AND delete_row = 'false'");
    list_not_work_model->select();

    ui->tableView_list_not_work->setModel(list_not_work_model);
    ui->tableView_list_not_work->hideColumn(0);
    ui->tableView_list_not_work->hideColumn(1);
    ui->tableView_list_not_work->hideColumn(8);
    ui->tableView_list_not_work->setFont(font_text);

    //dynamic_view
    model_dynamic_view->setTable("test.dynamic_view_patient");

    model_dynamic_view->setHeaderData(0,Qt::Horizontal,tr("id"));
    model_dynamic_view->setHeaderData(1,Qt::Horizontal,tr("medcard_id"));
    model_dynamic_view->setHeaderData(2,Qt::Horizontal,tr("Наблюдение \nоказывается с:"));
    model_dynamic_view->setHeaderData(3,Qt::Horizontal,tr("Вид наблюдения"));
    model_dynamic_view->setHeaderData(4,Qt::Horizontal,tr("Статус"));
    model_dynamic_view->setHeaderData(5,Qt::Horizontal,tr("Врач/Медсестра"));
    model_dynamic_view->setHeaderData(6,Qt::Horizontal,tr("Дата закрытия"));


    model_dynamic_view->setFilter("medcard_id = 0");
    model_dynamic_view->select();

    ui->tableView_dynamic_view->setModel(model_dynamic_view);

    ui->tableView_dynamic_view->hideColumn(0);
    ui->tableView_dynamic_view->hideColumn(1);
     ui->tableView_dynamic_view->hideColumn(7);

    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    ui->tableView_dynamic_view->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);

    ui->tableView_dynamic_view->setFont(font_text);

    model_visits_control->setTable("test.visits_control_patient");
    model_visits_control->setHeaderData(0,Qt::Horizontal,tr("id"));
    model_visits_control->setHeaderData(1,Qt::Horizontal,tr("medcard_id"));
    model_visits_control->setHeaderData(2,Qt::Horizontal,tr("Назначено"));
    model_visits_control->setHeaderData(3,Qt::Horizontal,tr("Назначил"));
    model_visits_control->setHeaderData(4,Qt::Horizontal,tr("Дата\nявки"));

    model_visits_control->setFilter("medcard_id=0");
    model_visits_control->select();

    ui->tableView_visits_control->setModel(model_visits_control);

    ui->tableView_visits_control->hideColumn(0);
    ui->tableView_visits_control->hideColumn(1);

    ui->tableView_visits_control->setFont(font_text);

    ui->tableView_visits_control->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableView_visits_control->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableView_visits_control->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tableView_visits_control->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableView_visits_control->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);


    model_sved_gosp->setTable("test.sved_gosp_patient");

    model_sved_gosp->setHeaderData(0,Qt::Horizontal,tr("id"));
    model_sved_gosp->setHeaderData(1,Qt::Horizontal,tr("medcard_id"));
    model_sved_gosp->setHeaderData(2,Qt::Horizontal,tr("Инициатор \nгоспитализации"));
    model_sved_gosp->setHeaderData(3,Qt::Horizontal,tr("Дата поступления"));
    model_sved_gosp->setHeaderData(4,Qt::Horizontal,tr("Дата выбытия"));
    model_sved_gosp->setHeaderData(5,Qt::Horizontal,tr("Добавил\nИзменил"));

    model_sved_gosp->setFilter("medcard_id = 0");
    model_sved_gosp->setSort(3,Qt::DescendingOrder);
    model_sved_gosp->select();

    ui->tableView_sved_gosp->setModel(model_sved_gosp);
    ui->tableView_sved_gosp->hideColumn(0);
    ui->tableView_sved_gosp->hideColumn(1);


    model_invalid->setHeaderData(0,Qt::Horizontal,tr("id"));
    model_invalid->setHeaderData(1,Qt::Horizontal,tr("medcard_id"));

    model_suicide->setTable("test.suicide");

    model_suicide->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model_suicide->setRelation(2, QSqlRelation("test.tsuicide", "id", "description"));
    model_suicide->setRelation(3, QSqlRelation("test.staff_view", "id", "staff_name"));
    model_suicide->setRelation(4, QSqlRelation("test.staff_view", "id", "staff_name"));

    model_suicide->setHeaderData(1, Qt::Horizontal, tr("Дата\nсовершения"));
    model_suicide->setHeaderData(2, Qt::Horizontal, tr("Тип"));
    model_suicide->setHeaderData(3, Qt::Horizontal, tr("Добавил"));
    model_suicide->setHeaderData(4, Qt::Horizontal, tr("Изменил"));

    ui->tableView_suicide->setModel(model_suicide);

    ui->tableView_suicide->hideColumn(0);
    ui->tableView_suicide->hideColumn(5);
    ui->tableView_suicide->hideColumn(6);
    ui->tableView_suicide->hideColumn(7);

    model_suicide->setFilter("medcard_id = 0");
    model_suicide->select();

    model_ood->setTable("test.ood");

    model_ood->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_ood->setRelation(4, QSqlRelation("test.staff_view", "id", "staff_name"));
    model_ood->setRelation(5, QSqlRelation("test.staff_view", "id", "staff_name"));

    model_ood->setHeaderData(2, Qt::Horizontal, tr("Дата\nсовершения"));
    model_ood->setHeaderData(3, Qt::Horizontal, tr("Статья\n УК РФ"));
    model_ood->setHeaderData(4, Qt::Horizontal, tr("Добавил"));
    model_ood->setHeaderData(5, Qt::Horizontal, tr("Изменил"));

    ui->tableView_ood->setModel(model_ood);
    ui->tableView_ood->hideColumn(0);
    ui->tableView_ood->hideColumn(1);
    ui->tableView_ood->hideColumn(6);
    ui->tableView_ood->hideColumn(7);

    model_ood->setFilter("medcard_id = 0");
    model_ood->select();
}

void MainWindow::load_settings_from_sql()
{
    #ifdef _WIN32
        load_translator();
    #endif
    load_area_list();
    load_rights_user();
    settings_ui();
    settings_maintoolbar();
    close_old_session();
    start_new_session();
    load_settings_program();
    load_count_visiting();
    set_table_param();

}
void MainWindow::load_translator()
{
    QTranslator * qt_translator = new QTranslator;
    if ( !qt_translator->load( "translations\\qt_ru.qm" ) )
    {
        delete qt_translator;
        qDebug()<<"Load_translation Failed";
    }
    qApp->installTranslator( qt_translator );
}
void MainWindow::load_area_list()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;

    if(db.open())
    {
        QStringList area_list;
        query.exec("SELECT area_id FROM test.servising_area where staff_id="+obj.staff_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
        while(query.next())
        {
            area_list.append(query.value(0).toString());
        }
        area_append_user = area_list.join(", ");
        obj.str_area_list = area_append_user;
        obj.servising_area = area_list;

    }

}
void MainWindow::load_rights_user()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    if(db.open())
    {
        query.exec("SELECT \
                   rights.rights\
                   FROM \
                   test.rights, \
                   test.staff, \
                   test.users\
                   WHERE \
                   staff.id = users.staff_id AND\
                users.id = rights.user_id AND\
                staff.status = '0' AND\
                staff.id = "+obj.staff_id);
                if(query.lastError().isValid())
        {
                qDebug()<<query.lastError();
                QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
    }
                while(query.next())
        {
                QString str = query.value(0).toString();
                for(int n = 0; str.count()>n;n++)
        {
            QString state = str.at(n);
            if(state=="0")
            {
                rights_user.append(false);
            }
            else if(state=="1")
            {
                rights_user.append(true);
            }
        }
    }
    obj.rights_user = rights_user;
}
}
void MainWindow::close_old_session()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    if(db.open())
    {
        query.exec("UPDATE test.session SET date_time_stop='"+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+"', state = 2 WHERE state = 1 AND staff = "+obj.staff_id);
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
        }
    }
}
void MainWindow::start_new_session()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    Objects_app obj;
    if(db.open())
    {
        QList<QHostAddress> myIpAddresses = QNetworkInterface::allAddresses();
        query.exec("INSERT INTO test.session(ip_adres, date_time_start, staff, state) VALUES ( '"+myIpAddresses[0].toString()+"', '"+QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")+"', '"+obj.staff_id+"', 1) RETURNING id");
        if(query.lastError().isValid())
        {
            qDebug()<<query.lastError();
            QMessageBox::warning(this,"Ошибка SQL","Произошла ошибка при обращении к базе данных");
            QMessageBox::warning(this,"Ошибка!", "Не получен номер сессии");
            exit(0);
        }
        while (query.next())
        {
            session_id = query.value(0).toString();
        }
    }
}
void MainWindow::load_settings_program()
{
    QSqlDatabase db = QSqlDatabase::database();

}
void MainWindow::load_count_visiting()
{
    Objects_app obj;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if(db.open())
    {
        query.exec("SELECT count(*) FROM test.visiting_big_table WHERE area_id in("+obj.servising_area.join(",")+") AND date_assigned = '"+QDate::currentDate().toString("dd.MM.yyyy")+"' AND group_disp_view in('0','1','2','3','4','5','6','7','8','9','11','12')");
        query.next();
        QString count = query.value(0).toString();
        ui->pushButton_visiting_today->setText("Посещений:"+count);
    }
}
void MainWindow::start_udp_server()
{

}

void MainWindow::sort_gosp(int i_collumn)
{
    //QSortFilterProxyModel * filter = new QSortFilterProxyModel();
    //filter->setSourceModel(model_sved_gosp);
    //filter->filterAcceptsColumn(i_collumn);

}
