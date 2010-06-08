#include <avr/io.h>
#include <stdio.h>
#include "a2d.h"
#include "Quadrotor.h"
#include "Motors.h"
#include "Gyros.h"
#include "Accelerometer.h"
#include "Hardware.h"

// gADC[0] -> gyro x
// gADC[1] -> gyro y
// gADC[1] -> gyro voltaje referencia

// u[0] -> referencia usuario giro en x
// u[1] -> referencia usuario giro en y
// u[2] -> referencia usuario giro en z
// u[3] -> referencia usuario altura

// motor[0] -> Sur
// motor[1] -> Oeste
// motor[2] -> Este
// motor[3] -> Norte

int8_t P[2]={0,0};
int8_t I[2]={0,0};
int8_t D[2]={0,0};

int8_t emergenciaSTOP=1;

int16_t I_Max[2]={0,0};
int16_t I_Min[2]={0,0};

int16_t u[4] = {0,0,0,0};
int16_t omega=0; 

void Control(void) // Timer.c llama esta funcion cada 1ms
{
    // variables PID
    static int16_t integrador[4];
    static int16_t e_old[4];
    int16_t e[4];
    // variables auxiliares
    uint8_t i;
    static uint8_t k=0;
    static uint8_t periodo=0;

    //cada 5ms el if es falso
    if(periodo++%5 != 0) {
        return;
    }
    
    GetGyros();
    GetAccelerometer();

    //cada 200ms ejecuta esto:
    if(periodo%50 == 1) {
        periodo=1;
        i=1;
        e[i] = joystick[0]/10 - angle[0];

        integrador[i] += I[i]*e[i];

        // Antienrrollamiento
        if(integrador[i] > I_Max[i])
            integrador[i] = I_Max[i];
        else if(integrador[i] < I_Min[i])
            integrador[i] = I_Min[i];

        omega = P[i]*e[i]/4 + integrador[i]/1000 + D[i]*(e[i] - e_old[i])/10;

        e_old[i] = e[i];
        if(k++%128==0) {
            printf("A: e: %d P: %d I:%d D:%d II:%d\n", e[1], P[1], I[1], D[1], integrador[1]);
            printf("G: e: %d P: %d I:%d D:%d II:%d\n", e[0], P[0], I[0], D[0], integrador[0]);
        }
        
    }


    // PID
    for(i=0; i<1; i++) {
        e[i] = gyro[i] - omega;

        integrador[i] += I[i]*e[i];

        // Antienrrollamiento
        if(integrador[i] > I_Max[i])
            integrador[i] = I_Max[i];
        else if(integrador[i] < I_Min[i])
            integrador[i] = I_Min[i];

        u[i] = P[i]*e[i]/10 + integrador[i]/1000 + D[i]*(e[i] - e_old[i])/10;

        e_old[i] = e[i];
    }

//    u[0] = joystick[0];
//    u[1] = joystick[1];
    u[2] = joystick[2];
    u[3] = joystick[3];


    if(emergenciaSTOP) {
        motor[0]=MOTORS_MIDDLE;
        motor[1]=MOTORS_MIDDLE;
        motor[2]=MOTORS_MIDDLE;
        motor[3]=MOTORS_MIDDLE;
        LED_ON( RED );
    } else {
        // Rotación de coordenadas + offset PWM
        motor[0] = u[3] - u[2] - u[1] + MOTORS_MIDDLE;
        motor[1] = u[3] + u[2] + u[0] + MOTORS_MIDDLE;
        motor[2] = u[3] + u[2] - u[0] + MOTORS_MIDDLE;
        motor[3] = u[3] - u[2] + u[1] + MOTORS_MIDDLE-200;
        motor[0]=MOTORS_MIDDLE;
        motor[3]=MOTORS_MIDDLE;
        LED_OFF( RED );
    }

    SetMotors();
}
