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
    int16_t gADC[3] = {0,0,0};
    static int16_t gADC_ant[3]={0,0,0};
    uint8_t i,j;

    // Captura ADC
    for(i=0; i<3; i++) {
//        for(j=0; j<3; j++) {
            gADC[i] += a2d_10(i+4);
//        }
//	gADC[i] /= 3;
    }

    // Conversion de unidades
    //
    // Medicion a V: 2.56/1023 V/bit
    // Quitando Zero g Bias Level: 1.5 V
    // mV a g: 0.3 V/g
    //
    // g = (ADC*(2.56/1023) - 1.5)/0.3
    //
    // Simplificando:
    // g = ADC/119.88 - 5

    // Solo necesitamos calcular el angulo asique no necesitamos que este en g
    // Para representar mejor el numero se multiplica por 119.88
    // 
    // g = ADC - 599.4
    //
    // Este es un valor calculado usando los valores tipicos del datasheet del sensor.
    // Este valor puede necesitar ajuste pero es una buena aproximacion. 

    for(i=0; i<3; i++) {
        accelerometer[i] = gADC[i] - 669;
    }

    angle[0] = -1*atan((double)accelerometer[1]/(double)accelerometer[2])*1000.0;
    angle[1] = atan((double)accelerometer[0]/(double)accelerometer[2])*1000.0;

    return;
}
