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

int main(void)
{
    int     i;
    int     led = 0;
    FILE   *u0;
    FILE   *u1;
    uint16_t datos[8][N_DATOS];
    uint8_t datos_periodo=0;

    InitHardware();

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

#if defined( __AVR_LIBC_VERSION__ )
    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
//    u1 = fdevopen( UART1_PutCharStdio, UART1_GetCharStdio );
#else
    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio, 0 );
//    u1 = fdevopen( UART1_PutCharStdio, UART1_GetCharStdio, 0 );
#endif

    printf( "*****\n" );
    printf( "***** Quadrotor program\n" );
    printf( "*****\n" );

    InitGyros();
    InitAccelerometer();
    InitMotors();

    while( 1 )
    {
        // Turn all of the LEDs off

        //LED_OFF( RED );
        LED_OFF( BLUE );
        LED_OFF( YELLOW );

        switch ( led )
        {
            case    0:  LED_ON( BLUE );     break;
            case    1:  LED_OFF( BLUE );    break;
        }

        if ( ++led >= 2 )
        {
            led = 0;
        }

        // Tick rate is 100/sec so waiting for 100 waits for 1 sec
//        printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", gyro[0], gyro[1], u[0], u[1], u[2], u[3], motor[0], motor[1], motor[2], motor[3], joystick[0], joystick[1], joystick[2], joystick[3]); 
        //printf("%d %d %d %d %d %d\n", gyro[0], u[0], u[3], motor[1], motor[2], joystick[3]);
/*
        datos[0][datos_periodo] = gyro[0];
        datos[1][datos_periodo] = gyro[1];
        datos[2][datos_periodo] = accelerometer[0];
        datos[3][datos_periodo] = accelerometer[1];
        datos[4][datos_periodo] = accelerometer[2];
        datos[5][datos_periodo] = angle[0];
        datos[6][datos_periodo] = angle[1];
        datos[7][datos_periodo] = omega;
        //printf("%d\n", datos_periodo);
        if(++datos_periodo % N_DATOS == 0) {
            datos_periodo = 0;
            for(i=0; i<N_DATOS; i++) {
//                printf("%d\n", datos[7][i]);
//                printf("%d %d %d %d %d %d %d %d\n", datos[0][i], datos[1][i], datos[2][i], datos[3][i], datos[4][i], datos[5][i], datos[6][i], datos[7][i]);
            }
        }
*/

        printf("%d %d", gyro[0], gyro[1]);
	printf(" %d %d %d", accelerometer[0], accelerometer[1], accelerometer[2]);
	printf(" %d %d\n", angle[0], angle[1]);

        for ( i = 0; i < 5; i++ ) 
        {
            WaitForTimer0Rollover();

            while ( UART0_IsCharAvailable() )
            {
                uint8_t ch[10] = "";
                uint8_t j=0, k;
                uint8_t checksum=0;

                LED_ON( YELLOW );

                do {
                    ch[j] = getchar();
                } while(ch[j++] != '#');

                if(j<=3) {
                    printf("!\n");
                    continue;
                }
                
                for(k=0; k<(j-2); k++) {
                    checksum += ch[k];
                }

                if(checksum != ch[j-2]) {
                    printf("!!\n");
                    continue;
                }

                if (( ch[0] == 'c' )) {
                    if(j-3 != 3) {
                        printf("!!!\n");
                        continue;
                    }
                    //c[1] -> axis
                    //c[2] -> value1
                    //c[3] -> value2
                    if(!(0 <= ch[1] && ch[1] <= 3)) {
                        printf("!!!!\n");
                        continue;
                    }
                    int16_t value = (ch[2] << 8) | ch[3];
                    joystick[ch[1]] = value;
//                    printf("joy[%d]=%d\n", ch[1], value);
                    continue;
                } else if((ch[0] == 's')) {
                    //c[1] -> axis
                    //c[2] -> constant
                    //c[3] -> value1
                    //c[4] -> value2
                    if(!(0 <= ch[1] && ch[1] <= 1)) continue;
                    int16_t value = (ch[3] << 8) | ch[4];

                    if(value<0) continue;

                    if((ch[2] == 'P')) {
                        pid[ch[1]].P = value;
                    } else if((ch[2] == 'I')) {
                        pid[ch[1]].I = value;
                    } else if((ch[2] == 'D')) {
                        pid[ch[1]].D = value;
                    } else if((ch[2] == 'W')) {
                        pid[ch[1]].windUpGuard = value;
                    }
                    continue;
                } else if((ch[0] == 'z')) {
                   zeroIntegralError();
                } else if((ch[0] == 'S')) {
                    if(ch[1] == 1) {
                        emergenciaSTOP = 1;
                        printf("STOP!\n");
                    } else if(ch[1] == 0) {
                        emergenciaSTOP = 0;
                        printf("START!\n");
                    }
                } else {
//                    printf("Error: %c?\n", ch[0]);
                }
            }

        }
    }

    return 0;
}

