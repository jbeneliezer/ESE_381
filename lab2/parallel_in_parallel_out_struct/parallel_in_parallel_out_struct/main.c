/*
 * parallel_in_parallel_out_struct.c
 *
 * Created: 2/18/2021 6:33:10 PM
 * Author : Judah Ben-Eliezerj
 */ 

#include <avr/io.h>

#define PORTA_INPUT_CONFIGURATION_gc 0x03
#define PORTC_INPUT_CONFIGURATON_gc 0xFC
#define PORTD_OUTPUT_CONFIGURATION_gc 0xFF

int main(void)
{
	
	PORTA.DIR &= PORTA_INPUT_CONFIGURATION_gc;
	PORTC.DIR &= PORTC_INPUT_CONFIGURATON_gc;
	PORTD.DIR |= PORTD_OUTPUT_CONFIGURATION_gc;

	uint8_t* ctrl_ptr = (uint8_t*)&PORTA.PIN2CTRL;

	for (uint8_t i = 0; i < 6; ++i) {
		*(ctrl_ptr + i) |= PORT_PULLUPEN_bm;
	}

	PORTC.PIN0CTRL |= PORT_PULLUPEN_bm;
	PORTC.PIN1CTRL |= PORT_PULLUPEN_bm;
	
	while (1)
	{
		PORTD.OUT = (PORTA.IN | PORTA_INPUT_CONFIGURATION_gc) | (PORTC.IN | PORTC_INPUT_CONFIGURATON_gc);
	}
}

