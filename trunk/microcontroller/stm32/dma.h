#ifndef DMA_H
#define DMA_H

#include <libopencm3/stm32/dma.h>
#include "adc.h"
#include "math.h"
#include "control.h"

extern u8 dma_flag;

void dma_setup();

#endif
