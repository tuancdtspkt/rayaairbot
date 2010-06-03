#include <avr/io.h>
#include "Gyros.h"

int16_t gyro[2] = {0,0};

void InitGyros() {
    return;
}

void GetGyros() {
    uint16_t gADC[3];
    uint8_t i;

    // Captura ADC
    for(i=0; i<3; i++) {
        gADC[ i ] = a2d_10( i );
    }

    // Ajustando en comparacion al voltaje de referencia 
    for(i=0; i<2; i++) {
        gyro[ i ] = ((int16_t)gADC[ i ] - (int16_t)gADC[ 2 ] + 10);
    }
    
    return;
}
