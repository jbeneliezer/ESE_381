/*
 * asynchronous_sw_read.c
 *
 * Created: 3/18/2021 5:47:32 PM
 * Author : Judah Ben-Eliezer
 */ 

#define BAUD_RATE 4800L													// baud rate.
#define F_CPU 4000000UL													// 4 MHz clock.
#include <avr/io.h>
#include <util/delay.h>

uint8_t USART_sw_read();												// declaration for read function.

char c;																	// global char variable.

int main(void)
{
    while (1) 
    {
		c = (char) USART_sw_read();										// read UART input to c.
    }
}

uint8_t USART_sw_read() {
	PORTB.DIRCLR = PIN1_bm;												// PB1 set as input.
	
	uint8_t d;															// bit time.
	if (BAUD_RATE == 4800L) {
		d = 205;
	} else if (BAUD_RATE == 9600L) {
		d = 102;
	} else if (BAUD_RATE == 19200L) {
		d = 50;
	} else return 0x00;

	uint8_t data = 0;
	uint8_t reading = 1;

	while (reading == 1) {
		while ((PORTB_IN & PIN1_bm) == 1) {}								// wait for falling edge.
		_delay_us(d/2);
		if ((PORTB_IN & PIN1_bm) != 0) continue;							// check for false start.
		_delay_us(d);														// delay for bit time.

		uint8_t i = 0;
		for (; i < 8; ++i) {
			data = (data >> 1) | ((PORTB_IN & PIN1_bm) << 6);						// read little endian input.
			_delay_us(d);													// delay for bit time.
		}
		reading = 0;
	}
	return data;
}
