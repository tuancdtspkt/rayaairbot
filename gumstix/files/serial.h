#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include "qextserialport.h"

class Serial : public QObject
{
    Q_OBJECT

public:
    Serial(QString serial_dev = "/dev/ttyS2");

public slots:
    void newCommandToSerial(QString s);

private slots:
    void serialRX();
    void serialTX(QString s);

signals:
    void newCommandFromSerial(QString s);

private:
    QextSerialPort * port;
    QString serial_data;


};

#endif // SERIAL_H
