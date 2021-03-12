/*
 * clk_main_32768Hz.c
 *
 * Created: 3/11/2021 8:25:04 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>

#define CLKCTRL_PDIV_disable 0x00												// mask for disable prescalar.

int main(void)
{
	PORTA.DIRSET = PIN7_bm;														// enable output on pin 7.

	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKCTRL_CLKSEL_OSC32K_gc;			// main clock enabled on CLKOUT, main clock set to internal 32.768 kHz oscillator.
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
    }
}




