#include "control.h"

float angle[2]={0,0};

void control_setup() {
	angle[0] = 0.0;
	angle[1] = 0.0;

    kalman[0].angle=0.0;
    kalman[0].dt=0.001;
    kalman[0].P[0][0]=1.0;
    kalman[0].P[0][1]=0.0;
    kalman[0].P[1][0]=0.0;
    kalman[0].P[1][1]=1.0;
    kalman[0].q_bias=0.0;
    kalman[0].rate=0.0;
    kalman[0].R_angle=0.3;
    kalman[0].Q_angle=0.001; //0.001
    kalman[0].Q_gyro=0.0000915527; //0.003;

    kalman[1].angle=0.0;
    kalman[1].dt=0.001;
    kalman[1].P[0][0]=1.0;
    kalman[1].P[0][1]=0.0;
    kalman[1].P[1][0]=0.0;
    kalman[1].P[1][1]=1.0;
    kalman[1].q_bias=0.0;
    kalman[1].rate=0.0;
    kalman[1].R_angle=0.3;
    kalman[1].Q_angle=0.01; //0.001
    kalman[1].Q_gyro=0.0000915527; //0.003;
}

void control() {
    s16 joystick_[4];
    s16 m[4];
//	s16 tmp;
	static u16 i=0;
//	unsigned char buf[30];

    joystick_[0] = joystick[0];
    joystick_[1] = joystick[1];
    joystick_[2] = joystick[2];
    joystick_[3] = joystick[3];

    if(joystick_[0]>100)
    	joystick_[0] = 100;
    if(joystick_[1]>100)
    	joystick_[1] = 100;
    if(joystick_[2]>100)
    	joystick_[2] = 100;
    if(joystick_[3]>100)
    	joystick_[3] = 100;

    if(joystick_[0]<-100)
    	joystick_[0] = -100;
    if(joystick_[1]<-100)
    	joystick_[1] = -100;
    if(joystick_[2]<0)
    	joystick_[2] = 0;
    if(joystick_[3]<-100)
    	joystick_[3] = -100;

    m[1] = joystick_[2] - joystick_[3]/4 + joystick_[0]/8;
	m[0] = joystick_[2] + joystick_[3]/4 - joystick_[1]/8;
	m[2] = joystick_[2] - joystick_[3]/4 - joystick_[0]/8;
	m[3] = joystick_[2] + joystick_[3]/4 + joystick_[1]/8;

	/*
	setMotor(0, m[0]);
	setMotor(1, m[1]);
	setMotor(2, m[2]);
	setMotor(3, m[3]);
	*/

	if((i++%500) == 0) {
/*
		buf[0] = CMD_GRAFICAR;

		buf[1] = (result_array[0] >> 8) & 0xFF;
		buf[2] = (result_array[0] >> 0) & 0xFF;

		buf[3] = (result_array[1] >> 8) & 0xFF;
		buf[4] = (result_array[1] >> 0) & 0xFF;

		buf[5] = (result_array[2] >> 8) & 0xFF;
		buf[6] = (result_array[2] >> 0) & 0xFF;

		buf[7] = (result_array[3] >> 8) & 0xFF;
		buf[8] = (result_array[3] >> 0) & 0xFF;

		buf[9] = (result_array[4] >> 8) & 0xFF;
		buf[10] = (result_array[4] >> 0) & 0xFF;

		send_cmd(buf, 10);

*/

		/*
		buf[10] = (result_array[5] >> 8) & 0xFF;
		buf[11] = (result_array[5] >> 0) & 0xFF;
		*/
		putint(result_array[0]);
		putstring("\t");
		putint(result_array[1]);
		putstring("\t");
		putint(result_array[2]);
		putstring("\t");
		putint(result_array[3]);
		putstring("\t");
		putint(result_array[4]);
		putstring("\t");
/*
		putint((int)(angle[0]*10.0));
		putstring("\t");
		putint((int)(angle[1]*10.0));

*/
/*

		putint(joystick[0]);
		putstring("\t");
		putint(joystick[1]);
		putstring("\t");
		putint(joystick[2]);
		putstring("\t");
		putint(joystick[3]);
*/
		putstring("\r\n");

	}
}
