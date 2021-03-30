/*
 * interrupt_echo_line.c
 *
 * Created: 3/18/2021 7:33:19 PM
 * Author : Judah Ben-Eliezer
 */ 

#define BAUD_RATE 4800UL													// baud rate.
#define F_CPU 4000000UL														// clock at 4 MHz.

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t USART_sw_read();													// read function declaration.
void USART_sw_write(char);													// write function declaration.

char c[80];																	// buffer.
uint8_t i = 0;

int main(void)
{
	PORTB.DIRCLR = PIN1_bm;													// set PB1 as input.
	PORTB.PIN1CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm;				// enable interrupt on falling edge of PB1.
	sei();																	// enable global interrupts.
	
    while (1) 
    {
		asm volatile ("nop");												// nop to avoid optimization deletion of while loop.
    }
}

ISR (PORTB_PORT_vect) {
	c[i++] = USART_sw_read();												// call USART_sw_read.
	if (c[i] == 0x0D) {														// check for CR.
		uint8_t j;
		for (j = 0; j <= i; ++j) {
			USART_sw_write(c[j]);											// write line.
		}
		i = 0;																// clear buffer
	}
	PORTB.INTFLAGS |= PIN1_bm;												// clear interrupt.
}

uint8_t USART_sw_read() {

	uint8_t d;																// bit time.
	if (BAUD_RATE == 4800UL) {
		d = 205;
	} else if (BAUD_RATE == 9600UL) {
		d = 102;
	} else if (BAUD_RATE == 19200UL) {
		d = 50;
	} else return 0x00;

	uint8_t data = 0;

	_delay_us(d/2);													
	if ((PORTB_IN & PIN1_bm) == PIN1_bm) return 0x00;						// check for false start.
	_delay_us(d);															// delay for bit time.

	uint8_t i;
	for (i = 0; i < 8; ++i) {
		data >>= data | ((PORTB_IN | PIN1_bm) << 6);						// read little endian input into data.
		_delay_us(d);														// delay for bit time.
	}

	return data;
}

void USART_sw_write(char c) {
	PORTB.DIRSET = PIN0_bm;													// set PB0 as output.
	uint8_t d;																// bit time.	
	if (BAUD_RATE == 4800L) {
		d = 205;
	} else if (BAUD_RATE == 9600L) {
		d = 102;
	} else if (BAUD_RATE == 19200L) {
		d = 50;
	} else return;

	uint8_t data = (uint8_t) c;

	PORTB_OUT = 0x00 | PIN0_bm;												// send start bit.
	_delay_us(d);															// delay for bit time.

	uint8_t i;
	for (i = 0; i < 8; ++i) {
		PORTB_OUT = data | PIN0_bm;											// send lsb of data.
		data >>= 1;															// shift data right.
		_delay_us(d);														// delay for bit time.
	}

	PORTB_OUT = PIN0_bm;													// send end bit.
	_delay_us(d);															// delay for bit time.
}

