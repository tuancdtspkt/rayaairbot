#ifndef RAYAAIRBOT_H
#define RAYAAIRBOT_H

#include <QObject>
#include <QtNetwork>

 class RayaAirBot : public QObject
 {
     Q_OBJECT

 public:
     RayaAirBot(QObject *parent = 0);

 private slots:
     void newConnection();
     void readyRead();
     void displayError(QAbstractSocket::SocketError socketError);
     void timeout();
     void newCommandProcess(quint8 comando, QString s);

 signals:
     void newCommand(quint8 comando, QString s);

 private:
     QTcpServer *tcpServer;
     QTcpSocket *tcpSocket;
     quint16 blockSize;

 };

 #endif
