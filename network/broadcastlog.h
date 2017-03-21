#ifndef BROADCASTLOG_H
#define BROADCASTLOG_H

#include <QObject>
#include <QtNetwork>
#include <QUdpSocket>
#include <QHostAddress>

class broadcastLog : public QObject
{
    Q_OBJECT
public:
    explicit broadcastLog(QObject *parent = 0);
    QUdpSocket * udpSocket;
    QList<QHostAddress> myIpAddresses;
signals:

public slots:
    void sendLogs(QString);
    void broadcastLogInit();
};

#endif // BROADCASTLOG_H
