/*
 * toggle_every_xxx_us.c
 *
 * Created: 3/11/2021 8:05:13 PM
 * Author : Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL															// 4 MHz clock

#include <avr/io.h>
#include <util/delay.h>

#define CLKCTRL_PDIV_disable 0x00												// mask for disable prescalar
#define DIPS_gm 0x30															// mask for input

int main(void)
{
	PORTA.DIRCLR = DIPS_gm;														// enable input on pin 7 and pin 6 of PORT A. 
	PORTC.DIRSET = PIN7_bm;													// enable output on pin 7 of PORT C.
	PORTA.PIN5CTRL |= PORT_PULLUPEN_bm;
	PORTA.PIN4CTRL |= PORT_PULLUPEN_bm;
	
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_disable;									// prescalar division disabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKSEL_OSCHF_gc;										// main clock set to internal high frequency oscillator.

	volatile uint8_t d;
	
    while (1) 
    {
		PORTC.OUT = PIN7_bm ^ PORTC.OUT;										// toggle pin 7 of PORT C

		d = ~(PORTA.IN | ~DIPS_gm);

		if (d == 0x00) {
			_delay_us(48);													// 52.08333 us delay.
		} else if (d == 0x10) {
			_delay_us(99);													// 104.1666 us delay.
		} else if (d == 0x20) {
			_delay_us(201);													// 208.333 us delay.
		}
    }
}



