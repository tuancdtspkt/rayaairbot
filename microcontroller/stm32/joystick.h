#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <cm3/common.h>
#include "gpio_.h"
#include "timer1.h"

extern s16 joystick[4];
extern s16 joystick_reader_counter;


void joystick_setup();
void joystick_capture();

#endif
