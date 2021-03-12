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
#define DIPS_gm 0x4F															// mask for input

int main(void)
{
	PORTA.DIRSET &= DIPS_gm;													// enable input on pin 7 and pin 6 of PORT A. 
	PORTC.DIRSET |= PIN7_bm;													// enable output on pin 7 of PORT C.

	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKSEL_OSCHF_gc;					// main clock enabled on CLKOUT, main clock set to internal high frequency oscillator.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_disable;									// prescalar division disabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLC = CLKCTRL_CFDSRC_CLKMAIN_gc | CLKCTRL_CFDEN_bm;			// clock failure source set to main clock, clock failure detection disabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKINTCTRL = CLKCTRL_INTTYPE_INT_gc;								// regular interrupt type.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = CLKCTRL_RUNSTBY_bm | CLKCTRL_FREQSEL_4M_gc | CLKCTRL_AUTOTUNE_bm;				// run on standby enabled, autotune enabled.

    while (1) 
    {
		PORTD.OUT = PIN7_bm ^ PORTD.OUT;										// toggle pin 7 of PORT C

		uint8_t delay = (~(PORTA.IN | DIPS_gm) >> 6) & 0x03;

		switch (delay) {
			case 0:
				_delay_us(49);													// 52.08333 us delay.
				break;
			case 1:
				_delay_us(99);													// 104.1666 us delay.
				break;
			case 2:
				_delay_us(199);													// 208.333 us delay.
				break;
			default:
				break;
		}
    }
}



