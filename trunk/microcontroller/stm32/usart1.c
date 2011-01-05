#include "usart1.h"
#include "delay.h"

struct fifo usart1_fifo_tx;
struct fifo usart1_fifo_rx;

#define FRAME_SIZE_TABLE_SIZE 9
char frame_size_table[] = { // CMD + ARG1 + ... + ARGn
		0x00,0x00,0x01,0x01,0x01,0x04,0x02,0x02,0x0A
};

char tx_restart = 1;

unsigned char usart1_rx_frames_available = 0;

int p1;
int p2;

void usart1_setup() {
	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 9600);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    USART_CR1(USART1) |= USART_CR1_RXNEIE;// | USART_CR1_TXEIE;

	/* Finally enable the USART. */
	usart_enable(USART1);

    fifo_init(&usart1_fifo_tx);
    fifo_init(&usart1_fifo_rx);

    frame_size_table[0] = 0;
    frame_size_table[1] = 0;
    frame_size_table[2] = 1;
    frame_size_table[3] = 1;
    frame_size_table[4] = 1;
    frame_size_table[5] = 4;
    frame_size_table[6] = 2;
    frame_size_table[7] = 2;
    frame_size_table[8] = 11;

    tx_restart = 1;

}

void usart1_parcer() {
    unsigned char c;
    unsigned char data;
    unsigned char data2[10];

    data = fifo_pop(&usart1_fifo_rx);

    switch(data) {
    case CMD_START:
        break;
    case CMD_LED:
    	data = fifo_pop(&usart1_fifo_rx);
    	if(data) {
    		LED_ON;
    	} else {
    		LED_OFF;
    	}
    	break;
    case CMD_CONFIG_PID:
        data = fifo_pop(&usart1_fifo_rx);
        switch(data) {
        case P1:
            data = fifo_pop(&usart1_fifo_rx);
            p1 = (data << 8) + fifo_pop(&usart1_fifo_rx);
            break;
        case P2:
            data = fifo_pop(&usart1_fifo_rx);
            p2 = (data << 8) + fifo_pop(&usart1_fifo_rx);
            break;
        }

        break;
    } // switch cmd

}

void usart1_isr() {

//	LED_TOGGLE;

	if (USART_SR(USART1) & USART_SR_RXNE) { // read interrupt
		unsigned char c;

//		USART_SR(USART1) &= ~USART_SR_RXNE; // clear interrupt
		c = USART_DR(USART1) & 0x1FF;
        usart1_new_data(c);

	}

	else if (USART_SR(USART1) & USART_SR_TXE) {
		if(usart1_fifo_tx.status != EMPTY) {
//			USART_SR(USART1) &= ~USART_SR_TXE; // clear interrupt
			USART_DR(USART1) = fifo_pop(&usart1_fifo_tx);
//			delay(1);
//			while ((USART_SR(USART1) & USART_SR_TXE) == 0);

		} else { // no more data to send
			USART_CR1(USART1) &= ~USART_CR1_TXEIE;
	        tx_restart = 1;
		}

	}

}

void usart1_new_data(unsigned char c) {
    static unsigned char start_flag = 0;
    static unsigned char esc_flag = 0;
    static unsigned char complete_frame_flag = 1;
    static unsigned char buf_i = 0;
    static unsigned char buf[50] = "";
    static unsigned int checksum = 0;
    static unsigned char frame_size = 0;
    unsigned char j = 0;

    if(esc_flag == 0) {
        // Buscando caracteres especiales
/*        if(c == EMERGENCY_CHAR) {
            esc_flag = 1; // proximo caracter no es especial
            return;
        } else */if(c == ESC_CHAR) {
            esc_flag = 1; // proximo caracter no es especial
            return;
        } else if(c == START_CHAR) {
        	if(!complete_frame_flag) {
                buf[0] = CMD_NAK;
        		send_cmd(buf, 1);
        	}
        	complete_frame_flag = 0;
            start_flag = 1;
            buf_i = 0;
            checksum = 0;
            return;
        }
    } else {
        esc_flag = 0;
    }

    if(start_flag == 1) { // Buscando tamaño de la trama
        start_flag = 0;
        if(c < FRAME_SIZE_TABLE_SIZE)
            frame_size = frame_size_table[c] + 1; // +1 => checksum
        else {
        	frame_size = 0;

            buf[0] = CMD_NAK;
    		send_cmd(buf, 1);
        }
    }

    if(buf_i < frame_size) {
        buf[buf_i++] = c; // guardando los datos

        if(frame_size == buf_i) { // trama completada
        	complete_frame_flag = 1;
            checksum = adding_excess(checksum);
            if(c != (unsigned char)checksum) {

                buf[0] = CMD_NAK;
        		send_cmd(buf, 1);
                // trama al agua
                return;
            }
            buf_i--; // quitando el checksum
            for(j=0; j < buf_i; j++) { // guardando en fifo
                fifo_push(&usart1_fifo_rx, buf[j]);
            }
            usart1_rx_frames_available++;
            buf[0] = CMD_ACK;
    		send_cmd(buf, 1);

        }

        checksum += c;
    } else {

    	if(complete_frame_flag == 0) {
            buf[0] = CMD_NAK;
    		send_cmd(buf, 1);
            // trama al agua
            return;
    	}
    }

}


unsigned char adding_excess(unsigned int checksum) {
    unsigned char checksum_8bit = 0;

    do {
        checksum_8bit += (checksum & 0xFF);
        checksum >>= 8;
    } while(checksum>0);

    return checksum_8bit;

}

void usart1_send(char c) {
/*
	USART_DR(USART1) = c;
	while ((USART_SR(USART1) & USART_SR_TXE) == 0);
*/
//	PC3_TOGGLE;

	if(usart1_fifo_tx.status != FULL) {
		fifo_push(&usart1_fifo_tx, c);


		if (tx_restart) { // If transmit interrupt is disabled, enable it
			tx_restart = 0;
//			while ((USART_SR(USART1) & USART_SR_TC) == USART_SR_TC);
			USART_CR1(USART1) |= USART_CR1_TXEIE;
		}

	} // else fifo full





//	PC3_TOGGLE;

}

void putint(int value)
{
	int i;
	int nr_digits = 0;
	char buffer[25];

	if(value == 0) {
		usart1_send('0');
	}

	if (value < 0) {
		usart1_send('-');
		 value = value * -1;
	}

	while (value > 0) {
		buffer[nr_digits++] = '0' + value%10;
		value = value/10;
	}

	nr_digits--;
	for (i=nr_digits; i>=0; i--) {
		usart1_send(buffer[i]);
	}
}

void putfloat(float value)
{
	putint((int) value);
	if (-1.0 < value && value < 0.0) {
		usart1_send('-');
		 value = value * -1;
	}
	usart1_send('.');
	if(value<0) value=-value;
	putint((int)((value - (int)value)*1000.0));
}

void putstring(const char *s)
{
    int i;
    for(i=0; s[i] != '\0'; ++i) {
    	usart1_send(s[i]);
    }

    return;
}

void putonechar(const char c)
{
	usart1_send(c);

    return;
}

void send_cmd(unsigned char buf[], unsigned char size) {
	unsigned char i,j;
	unsigned char buf2[50]; // start_char y checksum
	unsigned int checksum = 0;

	buf2[0] = START_CHAR;
	for(i=0,j=1; i<size; i++) {
		if(buf[i] == ESC_CHAR) {
			buf2[j++] = ESC_CHAR;
		} else if(buf[i] == START_CHAR) {
			buf2[j++] = ESC_CHAR;
		}
		buf2[j++] = buf[i];
		checksum += buf[i];
	}

	buf2[j++] = adding_excess(checksum);

	for(i=0; i<j; i++) {
		usart1_send(buf2[i]);
	}
}

