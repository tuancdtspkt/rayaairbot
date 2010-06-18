#include <avr/io.h>
#include <math.h>
#include "Accelerometer.h"

int16_t accelerometer[3] = {0,0,0};
int16_t angle[2] = {0,0};

void InitAccelerometer() {
    return;
}

void GetAccelerometer() {
    static double accelerometer_double[3]={0,0,0};
    int16_t gADC[3];
    static int16_t gADC_ant[3]={0,0,0};
    uint8_t i;

    // Captura ADC
    for(i=0; i<3; i++) {
        gADC[i] = a2d_10(i+4) - 341;
    }

    // Ajustando en comparacion al voltaje de referencia 
    for(i=0; i<3; i++) {
        accelerometer_double[i] += (double)(gADC[i] - accelerometer_double[i])/2.0;
        accelerometer[i] = accelerometer_double[i];
//        gADC_ant[i] = gADC[i];
//        accelerometer[i] = (1-A)*gADC[i] + A*accelerometer_anterior[i];
    //    accelerometer[i] = (accelerometer[i]*9+gADC[i])/10;
//        accelerometer[i] = accelerometer_anterior[i] + (gADC[i] - accelerometer_anterior[i])/50;
//        accelerometer_anterior[i] = accelerometer[i];
    }

    
    //angle[0] = atan2(accelerometer[2], accelerometer[1])*1800/M_PI+900;
    angle[0] = ((double)atan2(accelerometer[2], accelerometer[1])+(double)1.5708)*(double)100.0;
    angle[1] = atan2(accelerometer[2], accelerometer[0])*1800/M_PI+900;

    return;
}
