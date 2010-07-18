/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/

#include <avr/io.h>
#include <stdio.h>

#include "Hardware.h"
#include "Timer.h"
#include "UART.h"
#include "Quadrotor.h"
#include "Gyros.h"
#include "Accelerometer.h"
#include "Motors.h"
#include "Control.h"
#include "Pid.h"
//#include "a2d.h"

int16_t joystick[ 4 ] = {0,0,0,0}; 

#define N_DATOS 20

int16_t Has10msPassed(void);

void commandos(uint8_t ch) {
    static uint8_t buffer[80] = "";
    static uint8_t index_buffer = 0;
    uint8_t comando;
    uint8_t *argumentos;
    uint8_t argumentos_size;

    if(ch != '#') {
        buffer[index_buffer++] = ch;
    } else { // llego un comando nuevo
        uint8_t k;
        uint8_t checksum=0;

        // faltan argumentos
        if(index_buffer < 2) {
       	    index_buffer = 0;
            printf("!\n");
            return;
        }

        comando = buffer[0];
        argumentos = buffer + 1; // quitando el caracter que indica el comando
        argumentos_size = index_buffer - 2; // sacango el checksum
        index_buffer = 0;

//printf("comando: %c argumentos_size: %d\n", comando, argumentos_size);
//printf("comando: %c a1: %d ckechsum: %d\n", comando, argumentos[0]);
        
        // calculando checksum
        checksum=comando;
        for(k=0; k < argumentos_size; k++) {
            checksum += argumentos[k];
        }

        // checkeando checksum
        if(checksum != argumentos[argumentos_size]) {
            printf("!!\n");
            return;
        }

        // Procesando comandos
        if (( comando == 'c' )) {
            if(argumentos_size != 3) {
                printf("!!!\n");
                return;
            }
            //argumentos[0] -> axis
            //argumentos[1] -> value1
            //argumentos[2] -> value2
            if(!(0 <= argumentos[0] && argumentos[0] <= 3)) {
                printf("!!!!\n");
                return;
            }
            int16_t value = (argumentos[1] << 8) | argumentos[2];
//            if(argumentos[0]!=3)
//	    	value >>= 4;
            joystick[argumentos[0]] = value;
//                    printf("joy[%d]=%d\n", argumentos[0], value);
        } else if((comando == 's')) {
            //argumentos[0] -> axis
            //argumentos[1] -> constant
            //argumentos[2] -> value1
            //argumentos[3] -> value2
            if(!(0 <= argumentos[0] && argumentos[0] <= 1)) return;
            int16_t value = (argumentos[2] << 8) | argumentos[3];

            if(value<0) return;

            if((argumentos[1] == 'P')) {
                pid[argumentos[0]].P = value;
            } else if((argumentos[1] == 'I')) {
                pid[argumentos[0]].I = value;
            } else if((argumentos[1] == 'D')) {
                pid[argumentos[0]].D = value;
            } else if((argumentos[1] == 'W')) {
                pid[argumentos[0]].windUpGuard = value;
            }
        } else if((comando == 'z')) {
           zeroIntegralError();
        } else if((comando == 'S')) {
            if(argumentos[0] == 1) {
                emergenciaSTOP = 1;
                printf("STOP!\n");
            } else if(argumentos[0] == 0) {
                emergenciaSTOP = 0;
                printf("START!\n");
            }
        } /*else {
            printf("Error: %c?\n", comando);
        }*/
    }
}


int main(void)
{
//    uint8_t     i;
    int16_t     led = 0;
    FILE   *u0;
//    FILE   *u1;
    uint8_t count = 1;
    uint16_t j=0;

    InitHardware();

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

#if defined( __AVR_LIBC_VERSION__ )
//    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
    u0 = fdevopen( UART0_PutCharStdio, 0 );
//    u1 = fdevopen( UART1_PutCharStdio, UART1_GetCharStdio );
#else
//    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio, 0 );
    u0 = fdevopen( UART0_PutCharStdio, 0, 0 );
//    u1 = fdevopen( UART1_PutCharStdio, UART1_GetCharStdio, 0 );
#endif
    UART0_SetRxHandler(commandos);

    printf( "*****\n" );
    printf( "***** Quadrotor program\n" );
    printf( "*****\n" );

    InitGyros();
    InitAccelerometer();
    InitMotors();

    while(Has10msPassed() != -1);

    while(1) {

        switch ( led++ ) {
            case 0:
                LED_ON( BLUE );
                break;
            case 166:
                LED_OFF( BLUE );
                break;
            case 333:
                led = 0;
                break;
        }

//        j = gTickCount;
        Control();

/*
        if(j <= gTickCount)
            printf("%d\n", gTickCount - j);
        else
            printf("%d\n", 255-(j-gTickCount));
*/

        if(count++ >= 5) {
            count = 1;
/*
            printf("%d %d", gyro[0], gyro[1]);
            printf(" %d %d %d", accelerometer[0], accelerometer[1], accelerometer[2]);
            //printf(" %d %d\n", angle[0], angle[1]);
            printf(" %d %d", angle[0], angle[1]);
            printf(" %d %d\n", (int)theta[0]*1000, (int)theta[1]*1000);
*/
            printf("%d %d\n", (int16_t)(theta[0]*1000), (int16_t)(theta[1]*1000));
//            printf("%d = %d*%d/100 + %d/100*%d + %d*(%d-%d)/10\n", pid[0].r, pid[0].P, pid[0].e, pid[0].integratedError, pid[0].I, pid[0].D, pid[0].lastPosition, (int16_t)theta[0]*100);
//            printf("%d = %d*%d/100 + %d/100*%d + %d*(%d-%d)/10\n", pid[1].r, pid[1].P, pid[1].e, pid[1].integratedError, pid[1].I, pid[1].D, pid[1].lastPosition, pid[1].e);
//            printf("%d = %d*%d/100 + %d/100*%d + %d*(%d)/10  ", pid[0].r, pid[0].P, pid[0].e, pid[0].integratedError, pid[0].I, pid[0].D, -gyro[0]);
//            printf("%d = %d*%d/100 + %d/100*%d + %d*(%d)/10\n", pid[1].r, pid[1].P, pid[1].e, pid[1].integratedError, pid[1].I, pid[1].D, -gyro[1]);
        }

        while((j=Has10msPassed()) == -1);
//        printf("%d\n", j);
    }

    return 0;
}

int16_t Has10msPassed(void) {
    static int16_t nextCount = 0;
    tick_t tmpCount;

    if ( gTickCount >= nextCount) {
        tmpCount = nextCount;
        nextCount = (uint16_t)gTickCount + 10;
        if(nextCount>=256) nextCount -= 256;
        return (gTickCount - tmpCount);
    }

    return -1;

}

