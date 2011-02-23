#ifndef USART_H
#define USART_H

#include <stm32/usart.h>
#include "fifo.h"
#include "gpio_.h"
#include "nvic_.h"

extern struct fifo usart1_fifo_tx;
extern struct fifo usart1_fifo_rx;

extern unsigned char usart1_rx_frames_available;

extern char tx_restart;

void usart1_setup();
void usart1_send(char c);
void usart1_new_data(unsigned char c);
unsigned char adding_excess(unsigned int checksum);

void putonechar(const char c);
void putstring(const char *s);
void putstring_whiout_interrupt(const char *s);
void putfloat(float value);
void putint(int value);

void usart1_parcer();

void send_cmd(unsigned char buf[], unsigned char size);


#define START_CHAR 1
#define ESC_CHAR 0

#define CMD_ACK 2 // CMD
#define CMD_NAK 3 // CMD
#define CMD_START 4 // CMD
#define CMD_CONFIG_PID 5 // CMD + (P1 | I1 | D1 | P2 | I2 | D2) + VALUE_1 + VALUE_2
#define CMD_CONFIG_KALMAN 6 // CMD + (KG1 | KG2)
#define CMD_LED 7 // CMD + (ON | OFF)
#define CMD_GRAFICAR 8 // CMD + VALUE_1 + VALUE_2 + VALUE_1 + VALUE_2 + VALUE_1 + VALUE_2 + VALUE_1 + VALUE_2 + VALUE_1 + VALUE_2

#define P1 0
#define P2 1
#define I1 2
#define I2 3
#define D1 4
#define D2 5




#endif
