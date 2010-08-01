#if !defined( MOTORS_H )
#define MOTORS_H

// 3ms => 333Hz
#define TIMER_PERIOD  6000u
#define TIMER3_DIV    TIMER3_CLOCK_SEL_DIV_8 // 1, 8, 64, 256, 1024
#define TIMER1_DIV    TIMER1_CLOCK_SEL_DIV_8 // 1, 8, 64, 256, 1024


#define MOTORS_MAX     4000u //(2*(TIMER_PERIOD/20))
#define MOTORS_MIDDLE  3000u //((3*(TIMER_PERIOD/20))/2)
#define MOTORS_MIN     2000u //   (TIMER_PERIOD/20)

#define SERVO_1 3C
#define SERVO_2 3B
#define SERVO_3 3A
#define SERVO_4 1A

extern uint16_t motor[4];

void InitMotors(void);
void SetMotors(void);

#endif
