#include <avr/io.h>
#include "Gyros.h"

int16_t gyro[2] = {0,0};
int16_t bias = 0;

void InitGyros() {
    uint8_t j;

    bias = 0;

    for(j=0; j<20; j++) {
        bias += a2d_10(2);
    }

    bias /= 20;

    return;
}

void GetGyros() {
    int16_t gADC[2] = {0,0};
    uint8_t i, j;

    // Captura ADC
    for(i=0; i<2; i++) {
        for(j=0; j<3; j++) {
            gADC[i] += a2d_10(i);
        }
        gADC[i]/=3;
    }

    for(i=0; i<2; i++) {
        gyro[i] = gADC[i] - bias;
    }

    return;
}
