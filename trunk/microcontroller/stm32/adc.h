#ifndef ADC_H
#define ADC_H

#include <libopencm3/stm32/adc.h>

void adc_setup();

extern volatile uint16_t result_array_raw[5];
extern volatile int16_t result_array[5];
extern volatile float result_array_filtrado[5];
extern int adc_offset[4];
extern uint8_t enable_adc_offset_flag;

#endif
