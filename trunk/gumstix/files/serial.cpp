#include <cstdlib> // exit()
#include <unistd.h>

#include "serial.h"
#include "qextserialport.h"

Serial::Serial(QString serial_dev)
{   
    serial_data = "";

    port = new QextSerialPort(serial_dev, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD115200);
//    port->setBaudRate(BAUD57600);
//    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(serialRX()));
        qWarning("listening for data on %s", port->portName().toAscii().data());
        }
    else {
        qWarning("device failed to open: %s", port->errorString().toAscii().data());
        exit(1);
    }

}

void Serial::serialRX() {
    QByteArray bytes;

    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
    
    serial_data += bytes;

//    qWarning("bytes read: %d %s", bytes.size(), bytes.data());

    while(serial_data.contains('\n')) {
        int left = serial_data.indexOf('\n') + 1;
//        qWarning("bytes read: %d", serial_data.size());

        emit newCommandFromSerial(serial_data.left(left));

        serial_data = serial_data.right(serial_data.size() - left);
    }

}

void Serial::newCommandToSerial(QString s) {
    serialTX(s);
}

void Serial::serialTX(QString s) {
    quint8 checksum=0;

    if(s.indexOf('#')!=-1) {
        qWarning("Contiene #!!");
        return;
    }

    for(int i=0; i<s.size(); i++) {
        checksum += s.toAscii().data()[i];
    }

    s += checksum;
    s += '#';
//    qWarning("s!");

    while(s.size()) {
        port->write(s.toAscii(),1);
        usleep(10);
        s = s.right(s.size()-1);
    }
}

