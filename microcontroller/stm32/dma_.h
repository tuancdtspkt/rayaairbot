#ifndef DMA_H
#define DMA_H

#include <stm32/dma.h>
#include "adc_.h"
#include "math.h"
#include "control.h"

extern u8 dma_flag;

void dma_setup();

#endif
