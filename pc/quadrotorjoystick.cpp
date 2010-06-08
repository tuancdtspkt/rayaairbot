#include "quadrotorjoystick.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QTimer>

QuadrotorJoystick::~QuadrotorJoystick()
{
    delete ui;
}

void QuadrotorJoystick::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QuadrotorJoystick::QuadrotorJoystick(int joy_dev_, QString serial_dev_, QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
    emergenciaSTOP=1;
    ui->setupUi(this);

    joy_dev = joy_dev_;
    serial_dev = serial_dev_;

    joy = new QJoystick();
/*
    QStringList joylist = joy->joyList();
    for(int i=0; i<joylist.size();i++) {
        qWarning("Joystick %d, %s\n", i, joylist.at(i).toAscii().data() );
    }*/

    joy->open(joy_dev);

    port = new QextSerialPort(serial_dev, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD9600);
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
    }

    connect(joy, SIGNAL(axisValueChanged(int,int)), this, SLOT(axisValueChanged(int,int)));
    connect(joy, SIGNAL(buttonValueChanged(int,bool)), this, SLOT(buttonValueChanged(int,bool)));

    QTimer::singleShot(100, this, SLOT(initValues()));
}

void QuadrotorJoystick::buttonValueChanged(int boton, bool b) {
    if(b) {
        if(boton==0) {
            on_pushButton_clicked();
        } else if(boton==1) {
            initValues();
        }
    }
}

void QuadrotorJoystick::initValues(void) {    
    on_spinBox_P_valueChanged(ui->spinBox_P->value());
    on_spinBox_I_valueChanged(ui->spinBox_I->value());
    on_spinBox_D_valueChanged(ui->spinBox_D->value());
    on_spinBox_I_Max_valueChanged(ui->spinBox_I_Max->value());

    on_spinBox_P_2_valueChanged(ui->spinBox_P_2->value());
    on_spinBox_I_2_valueChanged(ui->spinBox_I_2->value());
    on_spinBox_D_2_valueChanged(ui->spinBox_D_2->value());
    on_spinBox_I_Max_2_valueChanged(ui->spinBox_I_Max_2->value());
}

void QuadrotorJoystick::serialRX() {
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());
//    qWarning("bytes read: %d", bytes.size());

    serial_data += bytes;

    while(serial_data.contains('\n')) {
        int left = serial_data.indexOf('\n') + 1;
        QStringList l = serial_data.left(left).split(" ");

        if(l.size() == 1) {

            //ui->label_M0->setText(l[0]);


/*
        if(l.size() == 8) {
//            qWarning("recibido-> %s", serial_data.left(left).toAscii().data());
            DataPlot *dataplot = qobject_cast<DataPlot *>(ui->widget_plot);
            if (dataplot) {
                ui->widget_plot->addData(l[0].toInt(), l[1].toInt());
            }
            DataPlot2 *dataplot2 = qobject_cast<DataPlot2 *>(ui->widget_plot_2);
            if (dataplot2) {
                ui->widget_plot_2->addData(l[2].toInt(), l[3].toInt(), l[4].toInt());
            }

            ui->label_u0->setText(QString::number(sqrt(pow(l[2].toInt(),2)+pow(l[3].toInt(),2)+pow(l[4].toInt(),2))));
            ui->label_u1->setText(l[5]);
            ui->label_u2->setText(l[6]);
            ui->label_M0->setText(l[7]);
*/
/*
        if(l.size() == 6) {
//            qWarning("recibido-> %s", serial_data.left(left).toAscii().data());
            DataPlot *dataplot = qobject_cast<DataPlot *>(ui->widget_plot);
            if (dataplot) {
                ui->widget_plot->addData(l[0].toInt(), 0);
            }

            ui->label_u0->setText(l[1]);
            ui->label_u3->setText(l[2]);
            ui->label_M1->setText(l[3]);
            ui->label_M2->setText(l[4]);
            ui->label_joy3->setText(l[5]);

*/
/*

        if(l.size() == 14) {
//            qWarning("recibido-> %s", serial_data.left(left).toAscii().data());
            DataPlot *dataplot = qobject_cast<DataPlot *>(ui->widget_plot);
            if (dataplot) {
                ui->widget_plot->addData(l[0].toInt(), l[1].toInt());
            }
            ui->label_u0->setText(l[2]);
            ui->label_u1->setText(l[3]);
            ui->label_u2->setText(l[4]);
            ui->label_u3->setText(l[5]);
            ui->label_M0->setText(l[6]);
            ui->label_M1->setText(l[7]);
            ui->label_M2->setText(l[8]);
            ui->label_M3->setText(l[9]);
            ui->label_joy0->setText(l[10]);
            ui->label_joy1->setText(l[11]);
            ui->label_joy2->setText(l[12]);
            ui->label_joy3->setText(l[13]);
*/
        } else {
            qWarning("recibido-> %s", serial_data.left(left-2).toAscii().data());
        }

        serial_data = serial_data.right(serial_data.size() - left);
    }

}

void QuadrotorJoystick::axisValueChanged(int axis, int value) {
    QString s;
//    qWarning("axis: %d value: %d", axis, value);

    s = "c";
    s += axis;
    value = (int)(((float)value)*1000.0/32676.0);

    switch(axis) {
    case 0:
        value = value/10;
        break;
    case 1:
        value = value/10;
        break;
    case 2:
        value = value/10;
        break;
    case 3:
        value = -value;
        break;
    }

    s += (value>>8)&0x00FF;
    s += value&0x00FF;

//    qWarning("enviando: axis: %d value1: %u value2: %u value: %d", axis, (value>>8)&0x00FF, value&0x00FF, value);
    qWarning("enviando: axis: %d value: %d", axis, value);
    serialTX(s);
    if(value==0) // reenviando por si no llega bien el 0 que es mas importante que el resto
        serialTX(s);

}

void QuadrotorJoystick::serialTX(QString s) {
    quint8 checksum=0;

    if(s.indexOf('#')!=-1) qWarning("Contiene #!!");

    for(int i=0; i<s.size(); i++) {
        checksum += s.toAscii().data()[i];
//        qWarning("char: %d", s.toAscii().data()[i]);
    }
//    qWarning("size-ch: %d", s.size());

    s += checksum;
    s += '#';
//    qWarning("size: %d", s.size());

//    qWarning("checksum: %d", checksum);

    if(!emergenciaSTOP)
        port->write(s.toAscii());
    else
        qWarning("STOP!!");
}

void QuadrotorJoystick::on_spinBox_P_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_P->setValue(value);
    s = "s";
    s += (char)0;
    s += "P";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: P0: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_P_valueChanged(int value)
{
    ui->spinBox_P->setValue(value);
}

void QuadrotorJoystick::on_spinBox_I_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_I->setValue(value);
    s = "s";
    s += (char)0;
    s += "I";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: I0: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_I_valueChanged(int value)
{
    ui->spinBox_I->setValue(value);

}

void QuadrotorJoystick::on_spinBox_D_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_D->setValue(value);
    s = "s";
    s += (char)0;
    s += "D";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: D0: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_D_valueChanged(int value)
{
    ui->spinBox_D->setValue(value);
}

void QuadrotorJoystick::on_spinBox_I_Max_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_I_Max->setValue(value);
    s = "s";
    s += (char)0;
    s += "W";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: M0: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_I_Max_valueChanged(int value)
{
    ui->spinBox_I_Max->setValue(value);
}

void QuadrotorJoystick::on_pushButton_clicked()
{
    if(emergenciaSTOP) {
        ui->pushButton->setText("Stop");
        ui->pushButton->setStyleSheet("background-color: rgb(255, 0, 0);");
        setEmergenciaSTOP(false);
    } else {
        ui->pushButton->setText("Start");
        ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 0);");
        setEmergenciaSTOP(true);
    }
}

void QuadrotorJoystick::setEmergenciaSTOP(bool b)
{
    QString s;
    s += 'S';
    if(b) {
        qWarning("Stop");
        s += (char)1;
        serialTX(s);
        serialTX(s);
        serialTX(s);
        serialTX(s);
        emergenciaSTOP = 1;
    } else {
        qWarning("Start");
        s += (char)0;
        emergenciaSTOP = 0;
        serialTX(s);
        serialTX(s);
        serialTX(s);
        serialTX(s);
    }
}





void QuadrotorJoystick::on_spinBox_P_2_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_P_2->setValue(value);
    s = "s";
    s += (char)1;
    s += "P";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: P1: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_P_2_valueChanged(int value)
{
    ui->spinBox_P_2->setValue(value);
}

void QuadrotorJoystick::on_spinBox_I_2_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_I_2->setValue(value);
    s = "s";
    s += (char)1;
    s += "I";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: I1: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_I_2_valueChanged(int value)
{
    ui->spinBox_I_2->setValue(value);
}

void QuadrotorJoystick::on_spinBox_D_2_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_D_2->setValue(value);
    s = "s";
    s += (char)1;
    s += "D";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: D1: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_D_2_valueChanged(int value)
{
    ui->spinBox_D_2->setValue(value);
}

void QuadrotorJoystick::on_spinBox_I_Max_2_valueChanged(int value)
{
    QString s;
    ui->horizontalSlider_I_Max_2->setValue(value);
    s = "s";
    s += (char)1;
    s += "W";
    s += (value>>8)&0x00FF;
    s += value&0x00FF;
    qWarning("enviando: M1: X value: %d", value);
    serialTX(s);
}

void QuadrotorJoystick::on_horizontalSlider_I_Max_2_valueChanged(int value)
{
    ui->spinBox_I_Max_2->setValue(value);
}
