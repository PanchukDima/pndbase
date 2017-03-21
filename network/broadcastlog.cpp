#include "broadcastlog.h"

broadcastLog::broadcastLog(QObject *parent) : QObject(parent)
{

}
void broadcastLog::broadcastLogInit()
{
     udpSocket = new QUdpSocket();
     myIpAddresses = QNetworkInterface::allAddresses();
}

void broadcastLog::sendLogs(QString log)
{
    QByteArray datagram = "Broadcast message{split}" + myIpAddresses[0].toString().toUtf8() + "{split}" + log.toUtf8();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, 45454);
}
