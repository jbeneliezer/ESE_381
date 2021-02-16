/*
 * PB1_and_PB2_control_LED.c
 *
 * Created: 2/12/2021 3:22:33 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>


int main(void)
{
	VPORTA_DIR = 0x00;
	VPORTB_DIR = 0x20;
	PORTA_PIN2CTRL = 0x08;
	PORTA_PIN5CTRL = 0x08;
	
    while (1) 
    {
		VPORTB_OUT = ~VPORTA_IN | (VPORTA_IN << 3);
    }
}

