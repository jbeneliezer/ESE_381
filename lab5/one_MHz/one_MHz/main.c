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
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_disable;									// prescalar division disabled.
    CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKSEL_OSCHF_gc;					// main clock enabled on CLKOUT, main clock set to internal high frequency oscillator.


    while (1) 
    {
    }
}

