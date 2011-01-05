#include "timer2.h"

void timer2_setup()
{
    // Timer 2 a 5kHz
	/* Set timer start value */
	TIM_CNT(TIM2) = 1;

	/* Set timer prescaler. 72MHz/144 => 500000 counts per second */
//	TIM_PSC(TIM2) = 144;  // 1kHz
	TIM_PSC(TIM2) = 144;  // 0.1 seg

	/* End timer value. If this value is reached an interrupt is generated */
	TIM_ARR(TIM2) = 500; // 1kHz
//	TIM_ARR(TIM2) = 2000; // 250 Hz

	/* Update interrupt enable */
	TIM_DIER(TIM2) |= TIM_DIER_UIE;

	/* Start timer */
	TIM_CR1(TIM2) |= TIM_CR1_CEN;
}

// trigger ADC
void tim2_isr()
{
	adc_start_conversion_regular(ADC1);

	/* clear interrrupt flag */
	TIM_SR(TIM2) &= ~TIM_SR_UIF;
}
