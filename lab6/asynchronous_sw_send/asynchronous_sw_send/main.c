/*
 * asynchronous_sw_send.c
 *
 * Created: 3/18/2021 4:31:07 PM
 * Author : Judah Ben-Eliezer
 */ 

#define BAUD_RATE 4800UL													// baud rate.
#define F_CPU 4000000UL														// 4 MHz clock.
#include <avr/io.h>
#include <util/delay.h>

void USART_sw_write(char);													// declaration for write function.

int main(void)
{
    while (1) 
    {
		USART_sw_write("A");												// write character.
		_delay_ms(1);														// delay 1 ms.
    }
}

void USART_sw_write(char c) {
	PORTB.DIRSET = PIN0_bm;													// set PB0 as output.
	uint8_t d;																// bit time.	
	if (BAUD_RATE == 4800L) {
		d = 48;
	} else if (BAUD_RATE == 9600L) {
		d = 99;
	} else if (BAUD_RATE == 19200L) {
		d = 201;
	} else return 0x00;

	uint8_t data = (uint8_t) c;

	PORTB_OUT = 0x00 | PIN0_bm;												// send start bit.
	_delay_us(d);															// delay for bit time.

	uint8_t i;
	for (i = 0; i < 8; ++i) {
		PORTB_OUT = data | PIN0_bm;											// send lsb of data.
		data >>= data;														// shift data right.
		_delay_us(d);														// delay for bit time.
	}

	PORTB_OUT = PIN0_bm;													// send end bit.
	_delay_us(d);															// delay for bit time.
}

