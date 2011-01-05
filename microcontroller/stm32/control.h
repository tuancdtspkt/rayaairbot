#ifndef CONTROL_H
#define CONTROL_H

#include "adc.h"
#include "usart1.h"
#include "joystick.h"
#include "kalman.h"

extern float angle[2];

void control_setup();
void control();

#endif
