#ifndef DELAY_H
#define DELAY_H

#include "timer1.h"

void delay_1ms();
void delay_ms(u16 ms);

#define delay(x) delay_ms(x)

#endif
