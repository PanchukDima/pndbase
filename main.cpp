#include "mainwindow.h"
#include "logs_save.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#include <QApplication>
#include <QDebug>
#include <QSql>
#include <QDateTime>
#include <QtDebug>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStyleFactory>
#include <QSplashScreen>



#include <Objects/User/objects_app.h>
#include <signal.h>
#include <network/broadcastlog.h>

void handler_sigsegv(int signum)
{
    logs_save logs;
#ifdef _WIN32
    MessageBoxA(NULL,"SIGSEGV Error!","POSIX Signal",MB_ICONSTOP);
#endif
    // открепить обработчик и явно завершить приложение
    logs.error_log("SIGSEGV Error! POSIX Signal " + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
    signal(signum, SIG_DFL);

}

void handler_sigfpe(int signum)
{
    logs_save logs;
#ifdef _WIN32
    MessageBoxA(NULL,"SIGFPE Error!","POSIX Signal",MB_ICONSTOP);
#endif
    // открепить обработчик и явно завершить приложение
    logs.error_log("SIGFPE Error! POSIX Signal" + QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
    signal(signum, SIG_DFL);

}
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
        broadcastLog netlog;
        netlog.broadcastLogInit();
        #ifdef _WIN32
        QFile fMessFile(qApp->applicationDirPath() + "/logs/myProjectLog"+QDate::currentDate().toString("_MMM_yyyy")+".log");
        #endif
        #ifdef __linux__
            QFile fMessFile("/var/log/BDPatient/"+QDate::currentDate().toString("_MMM_yyyy")+".log");
        #endif
         if(!fMessFile.open(QIODevice::Append | QIODevice::Text)){
         return;
         }
         QString sCurrDateTime = "[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "]";
         QTextStream tsTextStream(&fMessFile);
         switch(type){
         case QtDebugMsg:
         tsTextStream << QString("myProjectLogDebug%1: %2\n").arg(sCurrDateTime).arg(msg);
         netlog.sendLogs(msg);
         break;
         case QtWarningMsg:
         tsTextStream << QString("myProjectLogWarning%1: %2\n").arg(sCurrDateTime).arg(msg);
         netlog.sendLogs(msg);
         break;
         case QtCriticalMsg:
         tsTextStream << QString("myProjectLogCritical%1: %2\n").arg(sCurrDateTime).arg(msg);
         netlog.sendLogs(msg);
         break;
         case QtFatalMsg:
         tsTextStream << QString("myProjectLogFatal%1: %2\n").arg(sCurrDateTime).arg(msg);
         netlog.sendLogs(msg);
         abort();
             break;
         default:
             break;
         }
         tsTextStream.flush();
         fMessFile.flush();
         fMessFile.close();
}

int main(int argc, char *argv[])
{
    Objects_app obj;
    #ifdef _WIN32
        obj.path_settings = "settings_user.ini";
    #endif
    #ifdef __linux__
        obj.path_settings = "/etc/BDPatient/settings_user.ini";
    #endif
    qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    MainWindow w;

    qDebug()<<"Application Start";
    w.showMaximized();
    return a.exec();



}
