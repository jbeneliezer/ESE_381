/*
 * blink_LED0.c
 *
 * Created: 2/12/2021 4:44:00 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>
#define F_CPU 4000000
#include <util/delay.h>

int main(void)
{
	PORTB.DIR |= PIN3_bm;
	
    while (1) 
    {
		PORTB.OUT |= PIN3_bm;
		_delay_ms(500);
		PORTB.OUT &= ~PIN3_bm;
		_delay_ms(500);
    }
}

