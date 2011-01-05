#include "joystick.h"

s16 joystick[4]={0,0,0,0};
s16 joystick_reader_counter=0;

void joystick_setup() {

	joystick[0]=0;
	joystick[1]=0;
	joystick[2]=0;
	joystick[3]=0;

}

void joystick_capture()
{
    static u8 n=0,j;
    static u16 time_previous = 0;
	static u16 joystick_[6] = {0,0,0,0,0,0};
    u16 time, time_delta;
    u8 i;

    time = READ_TIMER_1;

    joystick_reader_counter++; // para saber si se perdió la comunicación con el joystick

	if(time_previous < time) {
		time_delta = time - time_previous;
	} else {
		time_delta = 60000 - (time_previous - time);
	}

	if(n == 0) {
		if(time_delta>15000) {
			n=1;
		}
	} else if(n != 0) {

		if(1700 < time_delta && time_delta < 4300)
			joystick_[n-1]=time_delta;
		else {
			n=0;
		}

		n++;
		if(n>6) {
			if(2700 < joystick_[4] && joystick_[4] < 2799 && 2700 < joystick_[5] && joystick_[5] < 2799) {
				for(i=0; i<4; i++) {
					switch(i) {
					case 0:
						joystick[0] = joystick_[0];
						joystick[0] = 200*((s16)joystick_[0]-2707)/(3458-1907);
						break;
					case 1:
						joystick[1] = joystick_[1];
						joystick[1] = 200*((s16)joystick_[1]-2750)/(3465-1993);
						break;
					case 2:
						joystick[2] = joystick_[2];
						joystick[2] = 100*((s16)joystick_[2]-2118)/(4090-2118);
						break;
					case 3:
						joystick[3] = joystick_[3];
						joystick[3] = 200*((s16)joystick_[3]-3407)/(4099-2639);
						break;
					}
				}
			}
			n=0;
		}
	}

	time_previous = time;
}
