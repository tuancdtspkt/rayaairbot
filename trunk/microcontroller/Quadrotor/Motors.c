#include <avr/io.h>
//#include "Hardware.h"
#include "Timer.h"
#include "Motors.h"

#define SERVO__( ocr, reg )     TIMER_ ## ocr ## _ ## reg
#define SERVO_( ocr, reg )      SERVO__( ocr, reg )
#define SERVO( num, reg )       SERVO_( SERVO_ ## num, reg )

uint16_t motor[4] = {MOTORS_MIDDLE, MOTORS_MIDDLE, MOTORS_MIDDLE, MOTORS_MIDDLE};

// motor[0] -> Sur
// motor[1] -> Oeste
// motor[2] -> Este
// motor[3] -> Norte

void InitMotors() {
//    ICR3 = 40000u;
//    ICR3 = 40000u;

    ICR3 = TIMER_PERIOD;
    ICR1 = TIMER_PERIOD;
    SERVO( 1, OCR ) = MOTORS_MIDDLE;   // servo mid point
    SERVO( 2, OCR ) = MOTORS_MIDDLE;
    SERVO( 3, OCR ) = MOTORS_MIDDLE;
    //SERVO( 4, OCR ) = MOTOR_MIDDLE;
    SERVO( 4, OCR ) = MOTORS_MIN; // este controlador es distinto

    TCNT3 = 0;
    TCNT1 = 0;

    TCCR3A = ( 1 << WGM31 ) | ( 0 << WGM30 );
    TCCR3B = ( 1 << WGM33 )  | ( 1 << WGM32 ) | TIMER3_DIV;
//    TCCR3B = ( 1 << WGM33 )  | ( 1 << WGM32 ) | TIMER3_CLOCK_SEL_DIV_1;

    TCCR1A = ( 1 << WGM11 ) | ( 0 << WGM10 );
    TCCR1B = ( 1 << WGM13 )  | ( 1 << WGM12 ) | TIMER1_DIV;
//    TCCR1B = ( 1 << WGM13 )  | ( 1 << WGM12 ) | TIMER1_CLOCK_SEL_DIV_1;

    // Set the servo pins to COM mode 2 (COM_1 = 1, COM_0 = 0)
    // This causes the OCR pin to fall which the counter reaches the value
    // stored in the OCR register and causes the OCR pin to rise when the 
    // counter reaches TOP (gets reset to zero).

    SERVO( 1, TCCRA ) |= ( 1 << SERVO( 1, COM_1 ));
    SERVO( 2, TCCRA ) |= ( 1 << SERVO( 2, COM_1 ));
    SERVO( 3, TCCRA ) |= ( 1 << SERVO( 3, COM_1 ));
    SERVO( 4, TCCRA ) |= ( 1 << SERVO( 4, COM_1 ));

    // Set the servo pins to be outputs

    SERVO( 1, DDR ) |= SERVO( 1, MASK );
    SERVO( 2, DDR ) |= SERVO( 2, MASK );
    SERVO( 3, DDR ) |= SERVO( 3, MASK );
    SERVO( 4, DDR ) |= SERVO( 4, MASK );

    return;
}

void SetMotors() {
    uint8_t i;

    // No salir del rango del PWM
    for(i=0; i<4; i++) {
        if(motor[i] > MOTORS_MAX)
            motor[i] = MOTORS_MAX;
        else if(motor[i] < MOTORS_MIN)
            motor[i] = MOTORS_MIN;
    }

    // Seteando los PWM
    SERVO( 1, OCR ) = motor[0];
    SERVO( 2, OCR ) = motor[1];
    SERVO( 3, OCR ) = motor[2];
    //SERVO( 4, OCR ) = motor[3];
    // controlador distinto

//    motor[3] -= 100;
    if(motor[3]<MOTORS_MIDDLE) motor[3] = MOTORS_MIDDLE;
    SERVO( 4, OCR ) = (motor[3]-MOTORS_MIDDLE)*2 + MOTORS_MIN;

    return;
}
