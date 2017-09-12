#include "mainwindow_police.h"
#include "ui_mainwindow_police.h"

MainWindow_police::MainWindow_police(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_police)
{
    ui->setupUi(this);
    connect(ui->pushButton_load,SIGNAL(clicked()),SLOT(start_generate()));
}

MainWindow_police::~MainWindow_police()
{
    delete ui;
}
void MainWindow_police::load_data(QString file_name)
{

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QStringList medcard_id;
    QStringList destroy;
    ui->progressBar->setValue(0);
    ui->label->setText("Поиск по статусу пациента");
    if(db.open())
    {
        query.exec("SELECT dynamic_view.medcard_id FROM test.dynamic_view WHERE delete_row = false AND status = false AND group_disp_view in ('0','1','2','3','4','5','6','7')");
        while(query.next())
        {
            medcard_id.append(query.value(0).toString());
        }
    }
    ui->progressBar->setMaximum(medcard_id.count()-1);
    for(int i=0;medcard_id.count()>i;i++)
    {
        ui->progressBar->setValue(i);
        if(db.open())
        {
            query.exec("SELECT patient.status FROM test.patient, test.medcard WHERE medcard.patient_id = patient.id AND delete_row = false AND medcard.id = "+medcard_id[i]);
            while(query.next())
            {
                if(query.value(0).toInt()<99)
                {
                    destroy.append(medcard_id[i]);
                }
            }
        }
    }

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(destroy.count()-1);
    ui->label->setText("Очистка списка");
    for(int i =0; destroy.count()>i;i++)
    {
        ui->progressBar->setValue(i);
        for(int y=0;medcard_id.count()>y;y++)
        {
            if(medcard_id[y]==destroy[i])
            {
                medcard_id.removeAt(y);
            }
        }
    }
    ui->progressBar->setValue(0);
    ui->label->setText("Формирование документа");
    ui->progressBar->setMaximum(medcard_id.count()-1);
    QFile f(file_name);
    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        for(int i =0;medcard_id.count()>i;i++)
        {

            if(db.open())
            {
                query.exec("SELECT  concat(patient.fname,' ',patient.name,' ',patient.mname) FROM test.medcard, test.patient WHERE medcard.patient_id = patient.id AND medcard.id="+medcard_id[i]);
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";

                }
            }
            if(db.open())
            {

                query.exec("SELECT medcard.birthday FROM test.medcard WHERE medcard.id = "+medcard_id[i]);
                if(query.lastError().isValid())
                {
                    qDebug()<<query.lastError();
                }
                while(query.next())
                {
                    strList << "\""+query.value(0).toDate().toString("dd.MM.yyyy")+"\"";
                }
            }
            if(db.open())
            {
                query.exec("SELECT diagnos.code FROM (SELECT fixing_diagnos_date ,diagnos_id, medcard_id, rank() OVER (PARTITION by medcard_id ORDER by fixing_diagnos_date DESC) FROM  test.diagnos_patient WHERE delete_row = false AND id_parent is null) AS date_diagnos, test.diagnos WHERE rank = 1 AND diagnos.id = diagnos_id AND medcard_id ="+medcard_id[i]);
                while (query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }

            }
            if(db.open())
                        {
                            query.exec("SELECT concat(street.name, CASE WHEN address_patient.building ='' THEN '' ELSE ' корп.' END , address_patient.building, ' д.', address_patient.home, ' кв.', address_patient.flat) AS Street FROM test.address_patient , test.street WHERE street.id = address_patient.street_id AND medcard_id = "+medcard_id[i]);
                            while(query.next())
                            {
                                strList << "\""+query.value(0).toString()+"\"";
                            }
                        }
            if(db.open())
                        {
                            query.exec("SELECT CASE WHEN medcard.district_id ='1' THEN 'Выборгский' ELSE 'Калининский' END FROM test.medcard WHERE medcard.id = "+medcard_id[i]);
                            while(query.next())
                            {
                                strList << "\""+query.value(0).toString()+"\"";
                            }
                        }
            if(db.open())
            {
                query.exec("SELECT min FROM test.max_and_min_dynamic WHERE medcard_id = "+medcard_id[i]);
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }
            }

            ts << strList.join( ";" )+"\n";
            strList.clear();
            ui->progressBar->setValue(i);
        }
        ui->progressBar->setValue(0);
        ui->label->setText("Готово");
        f.close();
    }
}
void MainWindow_police::start_generate()
{
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     QString::fromUtf8("Сохранить файл"),
                                                     QDir::currentPath(),
                                                     "CSV (*.csv);;All files (*.*)");
    if(ui->checkBox->isChecked())
    {
        start_thread(file_name);
    }
    else
    {
       load_data(file_name);
    }

}
void MainWindow_police::start_thread(QString file_name)
{
    QThread* thread = new QThread;
    thread_1 = new thread_load_data;
    thread_1->file = file_name;
    thread_1->moveToThread(thread);
    connect(thread,SIGNAL(started()),thread_1,SLOT(start()));

    connect(thread_1, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread_1, SIGNAL(finished()), thread_1, SLOT(deleteLater()));
    connect(thread_1,SIGNAL(status(int)),SLOT(change_status(int)));
    connect(thread_1,SIGNAL(progress(int)),SLOT(change_progres(int)));
    connect(thread_1,SIGNAL(progress_max(int)),SLOT(change_progress_max(int)));

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
void MainWindow_police::change_status(int x)
{
    switch (x) {
    case 0:
        ui->label->setText("Поиск пациентов по статусу");
        break;
    case 1:
        ui->label->setText("Очистка списка");
        break;
    case 2:
        ui->label->setText("Формирование документа");
        break;
    case 3:
        ui->label->setText("Готово");
        break;
    }
}
void MainWindow_police::change_progres(int x)
{
    ui->progressBar->setValue(x);
}
void MainWindow_police::change_progress_max(int x)
{
    ui->progressBar->setMaximum(x);
}
void MainWindow_police::cancel_other()
{
    if(ui->checkBox->isChecked())
    {
        thread_1->finished();
        ui->checkBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        MainWindow_police::close();
    }
}
