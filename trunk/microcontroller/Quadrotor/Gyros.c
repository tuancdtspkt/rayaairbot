#include <avr/io.h>
#include "Gyros.h"

int16_t gyro[2] = {0,0};

void InitGyros() {
    return;
}

void GetGyros() {
    uint16_t gADC[3] = {0,0,0};
    uint8_t i, j;

    // Captura ADC
    for(i=0; i<3; i++) {
        for(j=0; j<3; j++) {
            gADC[i] += a2d_10(i);
        }
        gADC[i]/=3;
    }


    // Ajustando en comparacion al voltaje de referencia 
//    for(i=0; i<2; i++) {
//        gyro[ i ] = ((int16_t)gADC[ i ] - (int16_t)gADC[ 2 ] + 10);
//    }
    gyro[ 0 ] = ((int16_t)gADC[ 0 ] - (int16_t)gADC[ 2 ] + 13);
    gyro[ 1 ] = ((int16_t)gADC[ 1 ] - (int16_t)gADC[ 2 ] + 16);
    
    gyro[ 0 ] >>= 1;
    gyro[ 1 ] >>= 1;

    gyro[ 0 ] >>= 1;
    gyro[ 1 ] >>= 1;

    return;
}
