/*
 * A_to_Z_async_Tx.c
 *
 * Created: 3/25/2021 7:26:26 PM
 * Author : Judah Ben-Eliezer
 */ 

#define BAUD_RATE 9600UL
#define F_CPU 4000000UL

#include <avr/io.h>

char c;

int main(void)
{

	PORTB.DIRSET = PIN0_bm;																								// enable output on PB0.

	USART3.BAUD = (64 * F_CPU)/(16 * BAUD_RATE);																		// set baud rate.
	USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;	// Asynchronous mode, no parity bits, single stop bit, 8 bits data.
	USART3.CTRLB = USART_TXEN_bm;																						// enable transmission.


	while (1)
	{
		c = 'A';
		for (; c <= 'Z'; ++c) {
			while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}												// wait till buffer is empty.
			USART3.TXDATAL = c;																							// transmit c
		}
	}
}
