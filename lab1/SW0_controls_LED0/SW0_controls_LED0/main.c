/*
 * SW0_controls_LED0.c
 *
 * Created: 2/12/2021 2:56:14 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>


int main(void)
{
	VPORTB_DIR = 0x08;
	//PORTB_PIN2CTRL = 0x08;
	
    while (1) 
    {
		VPORTB_OUT = VPORTB_IN << 1;
    }
}

