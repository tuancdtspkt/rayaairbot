#ifndef GPIO_H
#define GPIO_H

#include <libopencm3/stm32/gpio.h>

void gpio_setup();

#define LED_ON gpio_clear(GPIOC,GPIO12)
#define LED_OFF gpio_set(GPIOC,GPIO12)
#define LED_TOGGLE gpio_toggle(GPIOC,GPIO12)
#define PC0_TOGGLE gpio_toggle(GPIOC,GPIO0)
#define PC1_TOGGLE gpio_toggle(GPIOC,GPIO1)
#define PC2_TOGGLE gpio_toggle(GPIOC,GPIO2)
#define PC3_TOGGLE gpio_toggle(GPIOC,GPIO3)


#endif
