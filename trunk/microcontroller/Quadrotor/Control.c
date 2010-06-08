#include <avr/io.h>
#include <stdio.h>
#include "a2d.h"
#include "Quadrotor.h"
#include "Motors.h"
#include "Gyros.h"
#include "Accelerometer.h"
#include "Hardware.h"
#include "Pid.h"

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

struct PID pid[2];

int8_t emergenciaSTOP=1;

int16_t u[4] = {0,0,0,0};
int16_t omega=0; 

void Control(void) // Timer.c llama esta funcion cada 1ms
{
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

    //cada 50ms ejecuta esto:
    if(periodo%50 == 1) {
        periodo=1;
        omega = updatePID(&pid[0], joystick[0], angle[0]);

        if(k++%128==0) {
            printf("A: P: %d I:%d D:%d II:%d\n", pid[0].p, pid[0].I, pid[0].D, pid[0].integratedError);
            printf("G: P: %d I:%d D:%d II:%d\n", pid[1].p, pid[1].I, pid[1].D, pid[1].integratedError);
        }
        
    }


    // PID
    u[0] = updatePID(&pid[0], omega, gyro[0]);


//    u[0] = joystick[0];
    u[1] = joystick[1];
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
