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
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_disable;									// prescalar division disabled.
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm | CLKCTRL_CLKSEL_OSC32K_gc;			// main clock enabled on CLKOUT, main clock set to internal 32.768 kHz oscillator.
	
    while (1) 
    {
    }
}




