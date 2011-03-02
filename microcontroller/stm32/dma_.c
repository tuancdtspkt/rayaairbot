#include "dma_.h"

u8 dma_flag = 0;

void dma_setup()
{
    //ADC
	/* no reconfig for every ADC group conversion */
        dma_enable_circular_mode(DMA1, DMA_CHANNEL1);
	/* the memory pointer has to be increased, and the peripheral not */
        dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
	/* ADC_DR is only 16bit wide in this mode */
        dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);
	/*destination memory is also 16 bit wide */
        dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
	/* direction is from ADC to memory */
        dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);
	/* get the data from the ADC data register */
        dma_set_peripheral_address(DMA1, DMA_CHANNEL1,(u32) &ADC_DR(ADC1));
	/* put everything in this array */
        dma_set_memory_address(DMA1, DMA_CHANNEL1, (u32) &result_array_raw);
	/* we convert only 3 values in one adc-group */
        dma_set_number_of_data(DMA1, DMA_CHANNEL1, 5);
	/* we want an interrupt after the 3 halfwords. so we can compute something afterwards. */
        dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL1);

	/* dma ready to go. waiting til the peripheral gives the first data */
        dma_enable_channel(DMA1, DMA_CHANNEL1);
    //END ADC
}

#define A 0.4
#define B 0.6

void dma1_channel1_isr()
{
    static float result_array_raw_filtrado[5] = {0,0,0,0,0};
	int i;

	/*
	for(i=0; i<5; i++) {
		result_array_raw[0+i] = result_array_raw[0+i] + result_array_raw[5+i+1] + result_array_raw[10+i]);
		result_array_raw[0+i] = result_array_raw[0+i]/3;
	}*/

	// Midiendo los offset
	if(enable_adc_offset_flag) {
		for(i=0; i<4; i++) {
			adc_offset[i] = (result_array_raw[i] + 3*adc_offset[i])/4;
		}
		adc_offset[4] = (adc_offset[2] + adc_offset[3])/2;
	}

	// Quitando el offset
	for(i=0; i<5; i++) {

		result_array[i] = (result_array_raw[i] - adc_offset[i]);
	}

	dma_flag = 1; // new data, for the loop in main()

    /* clear the interrupt flag */
	DMA_IFCR(DMA1) |= DMA_IFCR_CGIF1;
}
