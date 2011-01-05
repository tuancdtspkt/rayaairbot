#include "motors.h"

void motors_setup() {
	setMotor(0, 0);
	setMotor(1, 0);
	setMotor(2, 0);
	setMotor(3, 0);
}

void setMotor(u8 number, s16 value) {

	// limite de value 2000 y 4000
	if(100 < value)
		value = 100;
	if(value <= 0)
		value = 0;

//	value = -value;
	value *= 20;
	value += 2000;

	switch(number) {
	case 0:
	    PWM1 = value;
		break;
	case 1:
	    PWM2 = value;
		break;
	case 2:
	    PWM3 = value;
		break;
	case 3:
	    PWM4 = value;
		break;
	default:
		break;

	}
}
