#include "clock.h"
#include "gpio.h"
#include "exti.h"
#include "timer1.h"
#include "nvic.h"
#include "usart1.h"
#include "delay.h"
#include "adc.h"
#include "dma.h"
#include "kalman.h"
#include "joystick.h"

#define SERIAL_DEBUG 1

#define GYRO_GAIN 0.09f

void hard_fault_handler(void) {
    putstring("error!\r\n");

    while(1) {
		delay(100);
		LED_TOGGLE;
    }

}

int main(void)
{
	u16 port_status;
	u16 port_status_old;
	u16 co=0;
	float angle_tmp[2]={0.0, 0.0};
	unsigned char buf[2];

	clock_setup();
	gpio_setup();

	// SERIAL with PC
	usart1_setup();

	// PWM IN
	exti_setup(); // external interrupt
	timer1_setup(); // time

	// ADC Gyro and Acelerometer
	dma_setup(); // adc done
	adc_setup();
	timer2_setup(); // start adc

	// MOTORS
	timer4_setup();
	motors_setup();

	joystick_setup();
	control_setup();

	// Interrupt priority
	nvic_setup();

#if SERIAL_DEBUG
	putstring("\r\n\r\n");
	putstring("********************\r\n");
    putstring("    Rayabot V2.0    \r\n");
    putstring("********************\r\n");
#endif

	LED_TOGGLE;
	enable_adc_offset_flag=1;

	delay(1000);
	delay(1000);
	delay(1000);
	delay(1000);
	delay(1000);
	delay(1000);

	enable_adc_offset_flag=0;
	LED_TOGGLE;

	// CONTROL
	timer3_setup(); // control loop

	while (1) {

		if(dma_flag) {
			dma_flag = 0;
			/*
			result_array_filtrado[2] = A*(float)result_array[2] + B*result_array_filtrado[2];
			result_array_filtrado[3] = A*(float)result_array[3] + B*result_array_filtrado[3];

			result_array[2] = (int16_t)result_array_filtrado[2];
			result_array[3] = (int16_t)result_array_filtrado[3];
			*/

			// Filtros de Kalman
			KalmanStateUpdate(&kalman[0], (float)result_array[0]*GYRO_GAIN);
			KalmanStateUpdate(&kalman[1], (float)result_array[1]*GYRO_GAIN);

		//    if(integral_count_reset++%10000==0) integral=0;
		//    integral = integral + result_array[0]*GYRO_GAIN*0.001;


		//    g = sqrtf(powf((float)result_array[2],2) + powf((float)result_array[3],2) + powf((float)result_array[4],2));


		//    if(340 < g && g < 440) { // centro: 420 // Quitando mediciones malas
			  angle_tmp[0] = -(float)atan2f((float)result_array[2], sqrtf(powf((float)result_array[4],2) + powf((float)result_array[3],2)))*180.0/3.14159;
			  angle_tmp[1] =  (float)atan2f((float)result_array[3], sqrtf(powf((float)result_array[4],2) + powf((float)result_array[2],2)))*180.0/3.14159;

			  if(result_array[4] < 0) {
				  if(result_array[2] < 0) {
					  angle_tmp[0] = 180.0 - angle_tmp[0];
				  } else {
					  angle_tmp[0] = -180.0 - angle_tmp[0];
				  }

				  if(result_array[3] < 0) {
					  angle_tmp[1] = 180.0 - angle_tmp[1];
				  } else {
					  angle_tmp[1] = -180.0 - angle_tmp[1];
				  }
			  }

			  /*
			  angle_filtrado[0] = A*angle_tmp[0] + B*angle_filtrado[0];
			  angle_filtrado[1] = A*angle_tmp[1] + B*angle_filtrado[1];

			  angle[0] = KalmanUpdate(&kalman[0], angle_filtrado[0]);
			  angle[1] = KalmanUpdate(&kalman[1], -angle_filtrado[1]);
		*/
			  angle[0] = KalmanUpdate(&kalman[0], angle_tmp[0]);
			  angle[1] = KalmanUpdate(&kalman[1], -angle_tmp[1]);

		/*
			} else {
				angle[0] = kalman[0].angle;
				angle[1] = kalman[1].angle;
				gpio_toggle(GPIOC, GPIO12);	// LED on/off
			}
		*/
			  if(co++%100==0) {
				  if(!(20 < joystick_reader_counter && joystick_reader_counter < 50)) {
					  motors_setup();
					  joystick_setup();
				  }
				  /*putint(joystick_reader_counter);
				  putstring("\r\n");*/
				  joystick_reader_counter=0;
			  }
		}

		while(usart1_rx_frames_available) {
			usart1_rx_frames_available--;
			usart1_parcer();
		}

	} //loop infinitely waiting for interrupts

  return 0;
}

