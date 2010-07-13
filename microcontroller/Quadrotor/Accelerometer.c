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
        for(j=0; j<3; j++) {
            gADC[i] += a2d_10(i+4);
        }
	gADC[i] /= 3;
    }

    // Conversion de unidades
    //
    // Medicion a V: 2.56/1024 V/bit
    // Quitando Zero g Bias Level: 1.5 V
    // mV a g: 0.3 V/g
    //
    // g = (ADC*(2.56/1024) - 1.5)/0.3
    //
    // Simplificando:
    // g = ADC/120 - 5

    // Solo necesitamos calcular el angulo asique no necesitamos que este en g
    // Para representar mejor el numero se multiplica por 120
    // 
    // g = ADC - 600
    //
    // Este es un valor calculado usando los valores tipicos del datasheet del sensor.
    // Este valor puede necesitar ajuste pero es una buena aproximacion. 

    for(i=0; i<3; i++) {
        accelerometer[i] = gADC[i] - 600;
    }

    angle[0] = atan((double)accelerometer[1]/(double)accelerometer[2])*100;
    angle[1] = atan((double)accelerometer[0]/(double)accelerometer[2])*100;

    return;
}
