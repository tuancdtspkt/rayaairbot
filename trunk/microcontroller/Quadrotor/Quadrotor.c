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
//#include "a2d.h"

int16_t joystick[ 4 ] = {0,0,0,0}; 

#define N_DATOS 20

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
        if(index_buffer <= 2) {
       	    index_buffer = 0;
            printf("!\n");
            return;
        }

        comando = buffer[0];
        argumentos = buffer + 1; // quitando el caracter que indica el comando
        argumentos_size = index_buffer - 1; // sacango el checksum
        index_buffer = 0;
        
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
    int     i;
    int     led = 0;
    FILE   *u0;
    FILE   *u1;

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

    while(1) {

        switch ( led++ ) {
            case 0:
                LED_ON( BLUE );
                break;
            case 1:
                LED_OFF( BLUE );
                led = 0;
                break;
        }

        printf("%d %d", gyro[0], gyro[1]);
        printf(" %d %d %d", accelerometer[0], accelerometer[1], accelerometer[2]);
        printf(" %d %d\n", angle[0], angle[1]);

        for ( i = 0; i < 10; i++ ) { // 10[ms]
            WaitForTimer0Rollover(); // 1[ms]
        }
    }

    return 0;
}

