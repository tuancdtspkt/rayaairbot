#ifndef QUADROTORJOYSTICK_H
#define QUADROTORJOYSTICK_H

#include <QObject>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include "qjoystick.h"
#include "rayaairbot.h"

namespace Ui {
    class MainWindow;
}

class QuadrotorJoystick : public QMainWindow {
Q_OBJECT
public:
    QuadrotorJoystick(int joy_dev_ = 0, QString serial_dev_ = "/dev/ttyUSB0", QWidget *parent = 0);
    ~QuadrotorJoystick();
    QJoystick *joy;
    int joy_dev;
    QString serial_dev;
    QString serial_data;

    RayaAirBot *rayaairbot;



//signals:
protected:
    void changeEvent(QEvent *e);

public slots:
    void axisValueChanged(int axis, int value);
    void buttonValueChanged(int boton, bool b);
    void serialRX();
    void serialTX(QString s);
    void setEmergenciaSTOP(bool b);


private:
    Ui::MainWindow *ui;
    bool emergenciaSTOP;

private slots:
    void on_pushButton_send_s_clicked();
    void on_pushButton_send_l_clicked();
    void on_pushButton_clicked();
    void initValues(void);

    void on_horizontalSlider_I_Max_valueChanged(int value);
    void on_spinBox_I_Max_valueChanged(int );
    void on_horizontalSlider_D_valueChanged(int value);
    void on_spinBox_D_valueChanged(int );
    void on_horizontalSlider_I_valueChanged(int value);
    void on_spinBox_I_valueChanged(int );
    void on_horizontalSlider_P_valueChanged(int value);
    void on_spinBox_P_valueChanged(int );

    void on_horizontalSlider_I_Max_2_valueChanged(int value);
    void on_spinBox_I_Max_2_valueChanged(int );
    void on_horizontalSlider_D_2_valueChanged(int value);
    void on_spinBox_D_2_valueChanged(int );
    void on_horizontalSlider_I_2_valueChanged(int value);
    void on_spinBox_I_2_valueChanged(int );
    void on_horizontalSlider_P_2_valueChanged(int value);
    void on_spinBox_P_2_valueChanged(int );
};

#endif // QUADROTORJOYSTICK_H
