#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QTextStream>

#include <Objects/User/objects_app.h>

namespace {
QMutex logMutex;
QString logFilePath;

QString resolveSettingsPath()
{
    const QString envPath = qEnvironmentVariable("BDPATIENT_SETTINGS");
    const QStringList candidates = {
        envPath,
        QDir::current().filePath("settings_user.ini"),
        QDir(QCoreApplication::applicationDirPath()).filePath("settings_user.ini"),
        QDir(QCoreApplication::applicationDirPath()).filePath("../settings_user.ini"),
        QStringLiteral("/etc/BDPatient/settings_user.ini")
    };

    for (const QString &candidate : candidates) {
        if (!candidate.isEmpty() && QFileInfo::exists(candidate))
            return QFileInfo(candidate).canonicalFilePath();
    }

    return QDir::current().filePath("settings_user.ini");
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QMutexLocker locker(&logMutex);
    QFile file(logFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    const char *level = "INFO";
    switch (type) {
    case QtDebugMsg: level = "DEBUG"; break;
    case QtInfoMsg: level = "INFO"; break;
    case QtWarningMsg: level = "WARN"; break;
    case QtCriticalMsg: level = "ERROR"; break;
    case QtFatalMsg: level = "FATAL"; break;
    }

    QTextStream stream(&file);
    stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")
           << " [" << level << "] " << message;
    if (context.file)
        stream << " (" << context.file << ':' << context.line << ')';
    stream << '\n';
    stream.flush();
}
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("BDPatient");

    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (logDir.isEmpty())
        logDir = QDir::currentPath();
    QDir().mkpath(logDir + "/logs");
    logFilePath = logDir + "/logs/bdpatient-" + QDate::currentDate().toString("yyyy-MM") + ".log";
    qInstallMessageHandler(messageHandler);

    Objects_app::path_settings = resolveSettingsPath();
    qInfo() << "Application start";
    qInfo() << "Settings file:" << Objects_app::path_settings;
    qInfo() << "Log file:" << logFilePath;

    MainWindow window;
    window.showMaximized();
    const int result = app.exec();
    qInfo() << "Application exit" << result;
    return result;
}
