#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QtNetwork>

 class Tcp : public QObject
 {
     Q_OBJECT

 public:
     Tcp(QObject *parent = 0);

 public slots:
     void newCommandToTCP(quint8 comando, QString s);

 private slots:
     void newConnection();
     void readyRead();
     void displayError(QAbstractSocket::SocketError socketError);

 signals:
     void newCommandFromTCP(quint8 comando, QString s);

 private:
     QTcpServer *tcpServer;
     QTcpSocket *tcpSocket;
     quint16 blockSize;

 };

#endif // TCP_H
