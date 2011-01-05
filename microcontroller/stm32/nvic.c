#include "nvic.h"

void nvic_setup()
{
	nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
	nvic_set_priority(NVIC_HARD_FAULT_IRQ, 0);

	nvic_enable_irq(NVIC_EXTI0_IRQ);
	nvic_set_priority(NVIC_EXTI0_IRQ, 1);

	nvic_enable_irq(NVIC_USART1_IRQ);
	nvic_set_priority(NVIC_USART1_IRQ, 2);

	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);
	nvic_set_priority(NVIC_DMA1_CHANNEL1_IRQ, 3);

	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 4);

	nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 1);
}
