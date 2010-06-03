#include <QApplication>
#include "quadrotorjoystick.h"

int main( int argc, char **argv )
{
    QApplication a(argc,argv);

//    Joystick j(&a);
//    j.show();
qWarning("Comenzando el programa...");
    QuadrotorJoystick quadrotorJoystick;
    quadrotorJoystick.show();

    qWarning("Comenzando el programa...?");
    return a.exec();
}
