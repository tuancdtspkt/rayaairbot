#include <QCoreApplication>
#include <QDebug>
#include <QtNetwork>
#include <QTimer>

#include "rayaairbot.h"

RayaAirBot::RayaAirBot(QObject *parent)
 : QObject(parent)
{

 tcpServer = new QTcpServer(this);
 if (!tcpServer->listen(QHostAddress::Any, 4000)) {
     qDebug() << tr("No se pudo iniciar el servidor TCP. Error: %1.").arg(tcpServer->errorString());
     exit(1);
     return;
 }

 qDebug() << tr("Servidor TCP corriendo en puerto: %1.").arg(tcpServer->serverPort());

 connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

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

void RayaAirBot::newConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    blockSize = 0;

    qDebug() << "Nueva conexion entrante";

    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(newCommand(quint8,QString)), this, SLOT(newCommandProcess(quint8,QString)));

}

void RayaAirBot::newCommandFromSerial(QString s) {
    qDebug() << "Serial entrante: " << s;
    send('s', s);
}

void RayaAirBot::newCommandProcess(quint8 comando, QString s) {
    switch(comando) {
    case 's':
        qDebug() << "Serial saliente: " << s;
        emit newCommandToSerial(s);
        break;
    default:
        qDebug() << "Comando " << comando << " no está definido. Dato: " << s;
        break;
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

