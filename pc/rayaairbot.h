#ifndef RAYAAIRBOT_H
#define RAYAAIRBOT_H

#include <QObject>
#include <QtNetwork>

 class RayaAirBot : public QObject
 {
     Q_OBJECT

 public:
     RayaAirBot(QObject *parent = 0);

 public slots:
     void send(quint8 comando, QString s);

 private slots:
     void readyRead();
     void displayError(QAbstractSocket::SocketError socketError);
     void newCommandProcess(quint8 comando, QString s);
     void reconnect();

 signals:
     void newCommand(quint8 comando, QString s);

 private:
     QTcpSocket *tcpSocket;
     quint16 blockSize;

 };

 #endif
