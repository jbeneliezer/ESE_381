/*
 * two_to_four_decoder_bitwise_C_ops.c
 *
 * Created: 2/18/2021 7:21:01 PM
 * Author : Judah Ben-Eliezer 
 */ 

#include <avr/io.h>

#define INPUT_gc 0x1F;
#define OUTPUT_gc 0x0F;

int main(void)
{
	
	PORTA_DIR &= INPUT_gc;
	PORTD_DIR |= OUTPUT_gc;

	uint8_t G_ = PORTA_IN | ~PIN7_bm, B = PORTA_IN | ~PIN6_bm, A = PORTA_IN | ~PIN5_bm, Y0, Y1, Y2, Y3;

    while (1) 
    {
		Y0 = (G_ | (((A << 1) | B) << 1) >> 7) | 0xFE;
		Y1 = (G_ | (((~A << 1) | B) << 1) >> 6) | 0xFD;
		Y2 = (G_ | (((A << 1) | ~B) << 1) >> 5) | 0xFB;
		Y3 = (G_ | (((~A << 1) | ~B) << 1) >> 4) | 0xF7;
		
		PORTD_OUT = Y0 & Y1 & Y2 & Y3;
		
    }
}

