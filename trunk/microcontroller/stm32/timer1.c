#include "timer1.h"

void timer1_setup()
{
    // Timer 1 a 2MHz

	/* Set timer start value */
	TIM_CNT(TIM1) = 1;

	/* Set timer prescaler. 72MHz/36 => 2MHz counts per second */
	TIM_PSC(TIM1) = 36;  // 2MHz

	/* End timer value. If this value is reached an interrupt is generated */
	TIM_ARR(TIM1) = 60000; // 1kHz

	/* Update interrupt enable */
//	TIM_DIER(TIM1) |= TIM_DIER_UIE;

	/* Start timer */
	TIM_CR1(TIM1) |= TIM_CR1_CEN;

}
