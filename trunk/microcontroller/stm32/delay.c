#include "delay.h"

void delay_ms(u16 ms)
{
	volatile u16 i, j;
	for(; ms>0; ms--)
		for (i = 0; i < 500; i++)
			for (j = 0; j < 5; j++);

	/*
	u16 i;
	for(i=0; i<ms; i++)
		delay_1ms();
		*/
}

void delay_1ms()
{
	u16 time_start;
	volatile u32 time_stop;

	time_start = READ_TIMER_1;

	time_stop = (time_start + 20000) % 60000;

	if(time_stop > time_start)
		while(time_stop <= READ_TIMER_1);
	else
		while(time_stop >= READ_TIMER_1);
}
