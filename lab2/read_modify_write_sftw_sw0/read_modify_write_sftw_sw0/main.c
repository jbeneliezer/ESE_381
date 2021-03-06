/*
 * read_modify_write_sftw_sw0.c
 *
 * Created: 2/18/2021 6:37:20 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>

#define PORTA_INPUT_CONFIGURATION_gc 0x03
#define PORTC_INPUT_CONFIGURATON_gc 0xFC
#define PORTD_OUTPUT_CONFIGURATION_gc 0xFF

int main(void)
{
	
	PORTA_DIR &= PORTA_INPUT_CONFIGURATION_gc;
	PORTC_DIR &= PORTC_INPUT_CONFIGURATON_gc;
	PORTD_DIR |= PORTD_OUTPUT_CONFIGURATION_gc;
	PORTB_DIR &= ~PIN3_bm;

	uint8_t* ctrl_ptr = (uint8_t*)&PORTA_PIN2CTRL;

	for (uint8_t i = 0; i < 6; ++i) {
		*(ctrl_ptr + i) |= PORT_PULLUPEN_bm;
	}

	PORTC_PIN0CTRL |= PORT_PULLUPEN_bm;
	PORTC_PIN1CTRL |= PORT_PULLUPEN_bm;

	uint8_t out;

	while (1)
	{
		out = (PORTA_IN | PORTA_INPUT_CONFIGURATION_gc) & (PORTC_IN | PORTC_INPUT_CONFIGURATON_gc);
		PORTD_OUT = (PORTB_IN & PIN3_bm) ? (out | 0x3C) & ((out << 2) | 0xC3): out;
	}
}
