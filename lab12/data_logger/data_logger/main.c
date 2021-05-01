/*
 * data_logger.c
 *
 * Created: 4/29/2021 5:29:14 PM
 * Author : Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL
#define BAUD_RATE 9600UL
#define SEC_addr 0x00
#define MIN_addr 0x01
#define HOUR_addr 0x02

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "fsm_ui.h"

state present_state = idle;																								// initialize state variable to idle.

key get_key(unsigned char);

int main(void)
{
	/* Configure ADC */
	adc_init();

	/* Configure lcd display */
	init_lcd_dog();
	
	/* Configure SPI for DS1306 RTC */
	SPI_rtc_ds1306_config();
	
	/* Configure usart3 */		
	USART3.BAUD = (64 * F_CPU)/(16 * BAUD_RATE);																		// set baud rate.
	USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc | USART_CHSIZE_8BIT_gc;	// Asynchronous mode, no parity bits, single stop bit, 8 bits data.
	USART3.CTRLB = USART_RXEN_bm;																						// enable transmission, enable receiving.
	USART3.CTRLA = USART_RXCIE_bm;
	
	/* Configure interrupt for DS1306 1Hz output */
	PORTC.DIRCLR |= PIN2_bm;																							// enable input on PC2.
	PORTC.PIN2CTRL = PORT_ISC_LEVEL_gc;																					// enable interrupt on PC2.

	write_line(1, "hello");
	dig = 0;
	
	sei();																												// enable global interrupts	
	
    while (1) 
    {
		asm volatile ("nop");
    }
}

ISR (USART3_RXC_vect) {
	if ((USART3.STATUS & USART_RXCIF_bm) == USART_RXCIF_bm) {
		present_state = fsm_ui(present_state, get_key(USART3.RXDATAL));
	}
}

ISR (PORTC_PORT_vect) {
	/* display time */
	char* _time = bcd2int(read_RTC(HOUR_addr) | 0x3F);
	_time = strncat(_time, (char*) ":", 1);
	_time = strncat(_time, bcd2int(read_RTC(MIN_addr)), 8);
	_time = strncat(_time, (char*) ":", 1);
	_time = strncat(_time, bcd2int(read_RTC(SEC_addr)), 8);
	
	write_line(1, _time);
	PORTC.INTFLAGS |= PIN2_bm;
}

ISR (ADC0_RESRDY_vect) {
	/* display temp */
	uint16_t data = ADC0.RES;
	int temp = convert(data);
	write_line(2, bin2ascii(temp));
}

key get_key(unsigned char c)
{
	if (c == 's') return s;
	else if (c == 'h') return h;
	else if (c == 'm') return m;
	else if (c == 'e') return e;
	else if (c >= '0' && c <= '9')
	{
		dig = (int) c;
		return digit;
	}
	else if (c == 0x0D) return enter;
	else return eol;
}
