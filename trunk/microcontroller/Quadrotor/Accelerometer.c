#include <avr/io.h>
#include "Accelerometer.h"

int16_t accelerometer[3] = {0,0,0};

void InitAccelerometer() {
    return;
}

#define A 0.53349

void GetAccelerometer() {
    static int16_t accelerometer_anterior[3]={0,0,0};
    int16_t gADC[3];
    static int16_t gADC_ant[3]={0,0,0};
    uint8_t i;

    // Captura ADC
    for(i=0; i<3; i++) {
        gADC[i] = a2d_10(i+4) - 341;
    }

    // Ajustando en comparacion al voltaje de referencia 
    for(i=0; i<3; i++) {
        accelerometer[i] += (gADC[i] - accelerometer[i])/10;
//        gADC_ant[i] = gADC[i];
//        accelerometer[i] = (1-A)*gADC[i] + A*accelerometer_anterior[i];
    //    accelerometer[i] = (accelerometer[i]*9+gADC[i])/10;
//        accelerometer[i] = accelerometer_anterior[i] + (gADC[i] - accelerometer_anterior[i])/50;
//        accelerometer_anterior[i] = accelerometer[i];
    }
    
    return;
}
