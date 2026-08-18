#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

#include <core/application/application_context.h>

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

QString defaultLogDirectory()
{
    QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (dataDirectory.isEmpty())
        dataDirectory = QDir::currentPath();
    return QDir(dataDirectory).filePath("logs");
}

QString resolveLogDirectory(const QString &settingsPath)
{
    QSettings settings(settingsPath, QSettings::IniFormat);
    QString directory = settings.value("Logging/path").toString().trimmed();
    if (directory.isEmpty())
        directory = defaultLogDirectory();
    else if (QDir::isRelativePath(directory))
        directory = QFileInfo(settingsPath).dir().absoluteFilePath(directory);

    return QDir::cleanPath(directory);
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

    ApplicationContext::instance().paths().settingsFile = resolveSettingsPath();
    QString logDir = resolveLogDirectory(ApplicationContext::instance().paths().settingsFile);
    if (!QDir().mkpath(logDir)) {
        const QString fallbackDirectory = defaultLogDirectory();
        QDir().mkpath(fallbackDirectory);
        logDir = fallbackDirectory;
    }
    logFilePath = QDir(logDir).filePath(
        "bdpatient-" + QDate::currentDate().toString("yyyy-MM") + ".log");
    qInstallMessageHandler(messageHandler);

    qInfo() << "Application start";
    qInfo() << "Settings file:" << ApplicationContext::instance().paths().settingsFile;
    qInfo() << "Log file:" << logFilePath;

    MainWindow window;
    window.showMaximized();
    const int result = app.exec();
    qInfo() << "Application exit" << result;
    return result;
}
