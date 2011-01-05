#include <avr/io.h>
#include "Gyros.h"

int16_t gyro[2] = {0,0};
int16_t gyro_angle[2] = {0,0};

void InitGyros() {
    return;
}

void GetGyros() {
    int16_t gADC[2] = {0,0};
    uint8_t i, j;

    // Captura ADC
    for(i=0; i<2; i++) {
//        for(j=0; j<3; j++) {
            gADC[i] = a2d_10(i);
//        }
//        gADC[i]/=3;
    }

    gyro[0] = gADC[0] - 509;
    gyro[1] = gADC[1] - 499;

    gyro_angle[0] += gyro[0]/16;
    gyro_angle[1] += gyro[1]/16;

//    gyro[0] >>= 4;
//    gyro[1] >>= 4;

    return;
}
