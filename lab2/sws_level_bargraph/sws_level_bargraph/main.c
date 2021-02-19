/*
 * sws_level_bargraph.c
 *
 * Created: 2/18/2021 8:10:39 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>


int main(void)
{
	uint8_t* ptr = (uint8_t*) &PORTA.PIN2CTRL;
	
	uint8_t switches, leds;
	
	PORTD.DIR |= 0xFF;
	PORTA.DIR &= 0x03;
	PORTC.DIR &= 0xFC;
	
	for (uint8_t i = 0; i < 6; ++i) {
		*(ptr + i) |= PORT_PULLUPEN_bm;
	}	
	
	PORTC.PIN0CTRL |= PORT_PULLUPEN_bm;
	PORTC.PIN1CTRL |= PORT_PULLUPEN_bm;
	
    while (1) 
    {
		switches = (PORTA.IN & 0xFC) | (PORTC.IN & 0x03);
		leds = 0x00;
		
		for (uint8_t i = 0; i < 8; ++i, switches >>= 1) {
			if (switches & 0x01) {
				leds <<= 1;
				leds |= 1;
			}
		}
		PORTB.OUT = ~leds;
    }
}

