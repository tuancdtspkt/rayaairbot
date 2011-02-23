#include "adc_.h"

volatile uint16_t result_array_raw[5] = {0,0,0,0,0};
volatile int16_t result_array[5] = {0,0,0,0,0};
volatile float result_array_filtrado[5] = {0,0,0,0,0};
int adc_offset[5]={0,0,0,0,0};
uint8_t enable_adc_offset_flag = 0;

void adc_setup(void)
{
	int i;
	u8 channel_array[10];

	/* make shure it didnt run during config */
	adc_off(ADC1);

	adc_enable_scan_mode(ADC1); /*scan mode means we scan all channels of the group to the end */

	//adc_set_continous_conversion_mode(ADC1); /* means we scan the group the whole day til someone disable continous mode */
	adc_set_single_conversion_mode(ADC1); /* this means we scan the group only one time */

	adc_disable_discontinous_mode_regular(ADC1); /* discontinous mode means all channels in a group will be *
												  * scanned in fragments of discmod channel count */

	adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART); /* ADC could be triggered by timer events or SWSTART */

	adc_set_right_aligned(ADC1);
	adc_enable_temperature_sensor(ADC1);
	adc_set_conversion_time_on_all_channels(ADC1, ADC_SMPR_SMP_7DOT5CYC);

	/* enable adc */
	adc_on(ADC1);
	/* wait for adc starting up*/
	for (i = 0; i < 80000; i++);    /* Wait (needs -O0 CFLAGS). */

	/* calibration */
	adc_reset_calibration(ADC1);
	adc_calibration(ADC1);

	i=0;
	/* Select the channel(s) (up  to 16) we want to convert. */
	channel_array[i++] = 1; // 1
	channel_array[i++] = 2;
	channel_array[i++] = 3;
	channel_array[i++] = 4;
	channel_array[i++] = 5; // 5


	adc_set_regular_sequence(ADC1, 5, channel_array);

	/* we want to use it with dma */
	adc_enable_dma(ADC1);

	/* wait for adc starting up*/
	for (i = 0; i < 80000; i++);    /* Wait (needs -O0 CFLAGS). */
}
