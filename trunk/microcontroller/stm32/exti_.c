#include "exti_.h"
#include "usart1.h"

void exti_setup()
{
	exti_enable_request(EXTI0);
	exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING); // EXTI_TRIGGER_BOTH
	exti_select_source(EXTI0, GPIOB);
}

void exti0_isr(void)
{
	joystick_capture();
	//	putstring("exti0_isr\r\n");
    exti_reset_request(EXTI0);
}

