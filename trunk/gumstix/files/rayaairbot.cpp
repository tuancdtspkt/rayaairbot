#include "rayaairbot.h"

RayaAirBot::RayaAirBot(QObject *parent)
 : QObject(parent)
{

    serial = new Serial;
    tcp = new Tcp;

    connect(serial, SIGNAL(newCommandFromSerial(QString)), this, SLOT(newCommandFromSerial(QString)));
    connect(tcp, SIGNAL(newCommandFromTCP(quint8,QString)), this, SLOT(newCommandFromTcpProcess(quint8,QString)));
}

void RayaAirBot::newCommandFromSerial(QString s) {
//    qDebug() << "Serial entrante: " << s;
    tcp->newCommandToTCP('s', s);
}

void RayaAirBot::newCommandFromTcpProcess(quint8 comando, QString s) {
    switch(comando) {
    case 's':
//        qDebug() << "Serial saliente: " << s;
        serial->newCommandToSerial(s);
        break;
    default:
        qDebug() << "Comando " << comando << " no esta definido. Dato: " << s;
        break;
    }
}
