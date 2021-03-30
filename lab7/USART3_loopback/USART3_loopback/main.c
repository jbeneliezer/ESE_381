/*
 * USART3_loopback.c
 *
 * Created: 3/25/2021 5:52:08 PM
 * Author : Judah Ben-Eliezer
 */ 

	
#define BAUD_RATE 9600UL
#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char c;

int main(void)
{

	PORTB.DIRSET = PIN0_bm;																								// enable output on PB0.
	PORTB.DIRCLR = PIN1_bm;																								// enable intput on PB1.

	USART3.BAUD = (64 * F_CPU)/(16 * BAUD_RATE);																		// set baud rate.
	USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;	// Asynchronous mode, no parity bits, single stop bit, 8 bits data.
	USART3.CTRLB = USART_TXEN_bm | USART_RXEN_bm;																		// enable transmission, enable receiving.
	USART3.CTRLA = USART_RXCIE_bm | USART_LBME_bm;																		// enable RX complete interrupt, loopback enabled.
	
	sei();

	c = 'A';
	
	while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}														// wait till buffer is empty.
	USART3.TXDATAL = c;																									// transmit c

	while (1)
	{
		__asm("nop");
	}
}

ISR (USART3_RXC_vect) {
	if ((USART3.STATUS & USART_RXCIF_bm) == USART_RXCIF_bm) {
		c = USART3.RXDATAL;

		if (c >= 'A' && c <= 'Z') {
			while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}												// wait till buffer is empty.
			USART3.TXDATAL = (c + 0x20);																				// send lowercase character.jjkkjjj
		} else if (c >= 'a' && c < 'z') {
			while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}												// wait till buffer is empty.
			USART3.TXDATAL = (c - 0x1F);																				// send next uppercase character.
		} else if (c == 'z') {
			while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}												// wait till buffer is empty.
			USART3.TXDATAL = 'A';																						// send 'A'
		} else {
			while ((USART3.STATUS & USART_DREIF_bm) != USART_DREIF_bm){}												// wait till buffer is empty.
			USART3.TXDATAL = 'A';																						// send 'A'
		}
	}
}
