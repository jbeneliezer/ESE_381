/*
 * is_CLK_CPU_independent_of_CLK_PER.c
 *
 * Created: 3/11/2021 7:02:14 PM
 * Author : Judah Ben-Eliezer 
 */ 

#include <avr/io.h>

#define CLKCTRL_PDIV_enable 0x01												// mask for enabling prescalar division.

int main(void)
{
	PORTA.DIRSET = PIN7_bm;														// enable output on pin 7 of PORT A
	PORTC.DIRSET = PIN7_bm;														// enable output on pin 7 of PORT C

	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_enable | CLKCTRL_PDIV_4X_gc;				// prescalar division enabled, prescaler set to 4
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLC = CLKCTRL_CFDSRC_CLKMAIN_gc | CLKCTRL_CFDEN_bm;			// clock failure source set to main clock, clock failure detection enabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKINTCTRL = CLKCTRL_INTTYPE_INT_gc;								// regular interrupt type.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = CLKCTRL_RUNSTBY_bm | CLKCTRL_FREQSEL_4M_gc | CLKCTRL_AUTOTUNE_bm;				// run on standby enabled, autotune enabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKSEL_OSCHF_gc;					// main clock enabled on CLKOUT, main clock set to internal high frequency oscillator.


    while (1) 
    {
		PORTC.OUT = PIN7_bm ^ PORTC.OUT;										// toggle pin 7 of PORT C
    }
}


