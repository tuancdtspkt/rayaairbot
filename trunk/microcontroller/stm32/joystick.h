#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <libopencm3/cm3/common.h>
#include "gpio.h"
#include "timer1.h"

extern s16 joystick[4];
extern s16 joystick_reader_counter;


void joystick_setup();
void joystick_capture();

#endif
