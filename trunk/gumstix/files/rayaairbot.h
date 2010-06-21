#ifndef RAYAAIRBOT_H
#define RAYAAIRBOT_H

#include <QObject>
#include <QtNetwork>
#include "tcp.h"
#include "serial.h"

 class RayaAirBot : public QObject
 {
     Q_OBJECT

 public:
     RayaAirBot(QObject *parent = 0);

 public slots:

 private slots:
     void newCommandFromSerial(QString s);
     void newCommandFromTcpProcess(quint8 comando, QString s);

 signals:
     void newCommandToSerial(QString s);

 private:
     Tcp *tcp;
     Serial *serial;

 };

 #endif
