#include <QCoreApplication>
#include <QDebug>
#include <QtNetwork>
#include <QTimer>

#include "tcp.h"

Tcp::Tcp(QObject *parent)
 : QObject(parent)
{
 tcpSocket = new QTcpSocket;

 connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
 connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
 connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(reconnect()));

 reconnect();

}

void Tcp::reconnect() {
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost("192.168.11.10", 4000);

}

void Tcp::send(quint8 comando, QString s) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    out << (quint8)comando;
    out << s;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    tcpSocket->write(block);
    tcpSocket->write(block);
    tcpSocket->write(block);

}

void Tcp::readyRead()
 {
     QDataStream in(tcpSocket);
     in.setVersion(QDataStream::Qt_4_0);

     while(1) {
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
 }

void Tcp::displayError(QAbstractSocket::SocketError socketError)
 {
     switch (socketError) {
     case QAbstractSocket::RemoteHostClosedError:
         break;
     case QAbstractSocket::HostNotFoundError:
         qDebug() << tr("The host was not found. Please check the host name and port settings.");
         break;
     case QAbstractSocket::ConnectionRefusedError:
         qDebug() << tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.");
         exit(1);
         break;
     default:
         qDebug() << tr("The following error occurred: %1.").arg(tcpSocket->errorString());
     }
 }

