/*
 * adc.c
 *
 * Created: 4/30/2021 1:27:53 PM
 *  Author: Judah Ben-Eliezer
 */

#include <avr/io.h>
#include "adc.h"

void adc_init(void)
{
	VREF.ADC0REF = VREF_REFSEL_1V024_gc;
	ADC0.CTRLA = ADC_ENABLE_bm;
	ADC0.INTCTRL = ADC_RESRDY_bm;
}

int convert(uint16_t res)
{
	return (res * 2500)/1024 - 500;
}

char* bin2ascii(int t)
{
	char a[] = {(char) (((t / 10000) % 10) & 0xF),
			(char) (((t / 1000) % 10) & 0xF),
			(char) (((t / 100) % 10) & 0xF),
			(char) (((t / 10) % 10) & 0xF),
			(char) ((t % 10) & 0xF)};
	return a;
}
