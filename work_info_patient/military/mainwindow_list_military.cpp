#include "mainwindow_list_military.h"
#include "ui_mainwindow_list_military.h"

MainWindow_list_military::MainWindow_list_military(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_list_military)
{
    ui->setupUi(this);
    set_param_table();
    load_table();

    connect(ui->pushButton_export,SIGNAL(clicked()),SLOT(export_csv()));
    connect(ui->pushButton_close,SIGNAL(clicked()),SLOT(close()));
    connect(ui->pushButton_analize,SIGNAL(clicked()),SLOT(find_military()));
}

MainWindow_list_military::~MainWindow_list_military()
{
    delete ui;
}
void MainWindow_list_military::set_param_table()
{
    model = new QSqlTableModel();
    model->setTable("reports_library.military");
    ui->tableView->setModel(model);


}
void MainWindow_list_military::load_table()
{
    model->select();
}
void MainWindow_list_military::export_csv()
{
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     QString::fromUtf8("Сохранить файл"),
                                                     QDir::currentPath(),
                                                     "CSV (*.csv);;All files (*.*)");
    QFile f(file_name);
    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;

        strList << "\" \"";
        for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            strList << "\""+ui->tableView->model()->headerData(c, Qt::Horizontal).toString()+"\"";
        ts << strList.join( ";" )+"\n";

        for( int r = 0; r < ui->tableView->verticalHeader()->count(); ++r )
        {
            strList.clear();
            strList << "\""+ui->tableView->model()->headerData(r, Qt::Vertical).toString()+"\"";
            for( int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c )
            {
                strList << "\""+ui->tableView->model()->data(ui->tableView->model()->index(r, c), Qt::DisplayRole).toString()+"\"";
            }
            ts << strList.join( ";" )+"\n";
        }
        f.close();
    }
}
void MainWindow_list_military::find_military()
{
    QString file_name = QFileDialog::getSaveFileName(this,
                                                     QString::fromUtf8("Сохранить файл"),
                                                     QDir::currentPath(),
                                                     "CSV (*.csv);;All files (*.*)");
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QStringList medcard_id;
    QList <QString> destroy;
    ui->label->setText("Поиск в возрасте от 18 до 27");
    if(db.open())
    {
        query.exec("SELECT medcard.id FROM test.medcard, test.age_patients WHERE medcard.id = age_patients.id AND date_part <= 27 AND date_part >= 18 AND sex = true;");
        while(query.next())
        {
            medcard_id.append(query.value(0).toString());
        }
    }
    ui->progressBar->setMaximum(medcard_id.count()-1);
    ui->label->setText("Проверка статуса");
    for(int i=0; medcard_id.count()>i;i++)
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
    ui->label->setText("Удаление лишних записей");
    for(int y =0; destroy.count()>y; y++)
    {
        ui->progressBar->setValue(y);
        for(int i=0;medcard_id.count()>i;i++)
        {
            if(medcard_id[i] == destroy[y])
            {
                medcard_id.removeAt(i);
            }
        }
    }
    destroy.clear();
    QFile f(file_name);
    if( f.open( QIODevice::WriteOnly ) )
    {
        QTextStream ts( &f );
        QStringList strList;
        bool delete_str;
        ui->progressBar->setValue(0);
        ui->progressBar->setMaximum(medcard_id.count()-1);
        ui->label->setText("Формирование документов");
        for(int i=0; medcard_id.count()>i;i++)
        {
            ui->progressBar->setValue(i);
            delete_str=false;
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
                query.exec("SELECT group_disp_view FROM (SELECT on_date ,group_disp_view, medcard_id, rank() OVER (PARTITION by medcard_id ORDER by on_date DESC) FROM  test.dynamic_view WHERE delete_row = false AND status = false ) AS date_dynamic_view WHERE rank = 1 AND group_disp_view in ('0','1','2','3','4','5','6','7','10') AND medcard_id = "+medcard_id[i]);
                if(query.numRowsAffected()<1)
                {
                    delete_str = true;
                    strList << "\"   \"";
                }
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }
            }


            if(db.open())
            {
                query.exec("SELECT concat(street.name, CASE WHEN address_patient.building ='' THEN '' ELSE ' корп.' END , address_patient.building, ' д.', address_patient.home, ' кв.', address_patient.flat) AS Street FROM test.address_patient , test.street WHERE street.id = address_patient.street_id AND medcard_id = "+medcard_id[i]);
                if(query.numRowsAffected()<1)
                {
                    strList << "\"   \"";
                }
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
                query.exec("SELECT medcard.birthday FROM test.medcard WHERE medcard.id = "+medcard_id[i]);
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";

                }
            }

            if(db.open())
            {
                query.exec("SELECT diagnos.code FROM (SELECT fixing_diagnos_date ,diagnos_id, medcard_id, rank() OVER (PARTITION by medcard_id ORDER by fixing_diagnos_date DESC) FROM  test.diagnos_patient WHERE delete_row = false AND id_parent is null) AS date_diagnos, test.diagnos WHERE rank = 1 AND diagnos.id = diagnos_id AND medcard_id ="+medcard_id[i]);
                if(query.numRowsAffected()<1)
                {
                    strList.clear();
                    continue;
                }
                while(query.next())
                {
                    strList << "\""+query.value(0).toString()+"\"";
                }

            }
            ts << strList.join( ";" )+"\n";
            strList.clear();
        }
    }
    ui->progressBar->setValue(0);
    ui->label->setText("Готово");
    f.close();
}
