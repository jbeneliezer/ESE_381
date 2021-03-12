/*
 * one_MHz.c
 *
 * Created: 3/5/2021 9:09:18 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>

#define CLKCTRL_PDIV_disable 0x00												// mask for disabling prescalar division.

int main(void)
{
	PORTA_DIRSET = PIN7_bm;														// CLK_OUT on PA7.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKSEL_OSCHF_gc;					// main clock enabled on CLKOUT, main clock set to internal high frequency oscillator.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_disable;									// prescalar division disabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLC = CLKCTRL_CFDSRC_CLKMAIN_gc | CLKCTRL_CFDEN_bm;			// clock failure source set to main clock, clock failure detection enabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKINTCTRL = CLKCTRL_INTTYPE_INT_gc;								// regular interrupt type.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = CLKCTRL_RUNSTBY_bm | CLKCTRL_AUTOTUNE_bm;				// run on standby enabled, autotune enabled.

    while (1) 
    {
    }
}

