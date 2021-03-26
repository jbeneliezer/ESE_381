/*
 * USART3_asynch_test.c
 *
 * Created: 3/25/2021 6:23:48 PM
 * Author : Judah Ben-Eliezer
 */ 

#define BAUD_RATE 9600UL
#define F_CPU 4000000UL

#include <avr/io.h>

char c;

int main(void)
{

	PORTB.DIRSET = PIN0_bm;																								// enable output on PB0.

	USART3.BAUD = BAUD_RATE;																							// set baud rate.
	USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;	// asynchronous mode, no parity bit, 1 stop bit, 8 bits data.
	USART3.CTRLB = USART_TXEN_bm;																						// enable transmission.
	
	c = 'U';

	while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}														// wait for buffer to be empty.
	USART3.TXDATAL = c;

    while (1) 
    {
		// do nothing
    }
}
