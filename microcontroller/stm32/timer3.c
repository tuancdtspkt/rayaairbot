#include "timer3.h"

void timer3_setup()
{
    // Timer 3 a 100Hz

	/* Set timer start value */
	TIM_CNT(TIM3) = 1;

	/* Set timer prescaler. 72MHz/720 => 100kHz counts per second */
	TIM_PSC(TIM3) = 720;

	/* End timer value. If this value is reached an interrupt is generated */
	TIM_ARR(TIM3) = 100; // 1 kHz

	/* Update interrupt enable */
	TIM_DIER(TIM3) |= TIM_DIER_UIE;

	/* Start timer */
	TIM_CR1(TIM3) |= TIM_CR1_CEN;
}

// Envío de datos
void tim3_isr()
{
//    putstring("C");
	control();

	/* clear interrrupt flag */
	TIM_SR(TIM3) &= ~TIM_SR_UIF;
}
