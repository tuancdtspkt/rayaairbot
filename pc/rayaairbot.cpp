#include <QCoreApplication>
#include <QDebug>
#include <QtNetwork>
#include <QTimer>

#include "rayaairbot.h"

RayaAirBot::RayaAirBot(QObject *parent)
 : QObject(parent)
{
 tcpSocket = new QTcpSocket;

 connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
 connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
 connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(reconnect()));

 connect(this, SIGNAL(newCommand(quint8,QString)), this, SLOT(newCommandProcess(quint8,QString)));

 reconnect();

}

void RayaAirBot::reconnect() {
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost("localhost", 4000);

}

void RayaAirBot::send(quint8 comando, QString s) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << (quint8)comando;
    out << s;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    tcpSocket->write(block);

}

void RayaAirBot::newCommandProcess(quint8 comando, QString s) {
    switch(comando) {
    case 's': qDebug("s"); qDebug(s.toAscii().data()); break;
    case 'l': qDebug("l"); qDebug(s.toAscii().data()); break;
    default: break;
    }
}


void RayaAirBot::readyRead()
 {
     QDataStream in(tcpSocket);
     in.setVersion(QDataStream::Qt_4_0);

     if (blockSize == 0) {
         if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
             return;

         in >> blockSize;
     }

     if (tcpSocket->bytesAvailable() < blockSize)
         return;

     quint8 comando;
     in >> comando;
     QString s;
     in >> s;

     emit newCommand(comando, s);

     blockSize = 0;

 }

void RayaAirBot::displayError(QAbstractSocket::SocketError socketError)
 {
     switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
         qDebug() << tr("The host was not found. Please check the host name and port settings.");
         break;
     case QAbstractSocket::ConnectionRefusedError:
         qDebug() << tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.");
         break;
     default:
         qDebug() << tr("The following error occurred: %1.").arg(tcpSocket->errorString());
     }
 }

