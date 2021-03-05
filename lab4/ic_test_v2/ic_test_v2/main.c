/*
 * ic_test_v2.c
 *
 * Created: 3/4/2021 6:59:47 PM
 * Author : Judah Ben-Eliezer	
 */ 

#define  F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#define IN_COMB_00 0x00
#define IN_COMB_01 0x55
#define IN_COMB_10 0xAA
#define IN_COMB_11 0xFF
#define DIP_SW_gm 0xE0
#define START_PB_bm 0x08
#define BARGRAPH_gm 0x07
#define TIP_bm 0x04
#define PASS_bm 0x01
#define FAIL_bm 0x02
#define A_GATES_OUT_gm 0x1F
#define B_GATES_OUT_gm 0x30
#define PA_setup_gm 0x07
#define PB_setup_gm 0xCF
#define PC_setup_gm 0xFF
#define PD_setup_gm 0x07
#define PE_setup_gm 0x0F
#define PF_setup_gm 0xF7

const uint8_t stimulus[] = {IN_COMB_00, IN_COMB_01, IN_COMB_10, IN_COMB_11};
	
const uint8_t verify[5][4] = {{0x0F, 0x0F, 0x0F, 0x00},
							 {0x00, 0x00, 0x00, 0x0F},
							 {0x00, 0x0F, 0x0F, 0x0F},
							 {0x00, 0x0F, 0x0F, 0x00},
							 {0x0F, 0x0F, 0x0F, 0x00}};
												
uint8_t gate_type;

uint8_t i;

void test() {
	//turn DUT pin 14 on
	PORTE_OUT |= PIN3_bm;

	for (i = 0; i < 4; ++i) {
		PORTC_OUT = stimulus[i];
			
		_delay_loop_1(2);
			
		uint8_t a_in = (PORTA_IN & A_GATES_OUT_gm) >> 3 & 0x03;
		uint8_t b_in = (PORTB_IN & B_GATES_OUT_gm) >> 2 & 0x0C;
			
		if (!((a_in | b_in) == verify[gate_type][i])) break;
	}
		
	PORTE_OUT |= TIP_bm;
		
	if (i == 4) PORTE_OUT &= ~PASS_bm;
	else PORTE_OUT &= ~FAIL_bm;
	
	PORTC_OUT = stimulus[0];

	//turn DUT pin 14 off
	PORTE_OUT &= ~PIN3_bm;
}

int main(void)
{
	PORTA_DIRSET &= PA_setup_gm;
	PORTB_DIRSET &= PB_setup_gm;
	PORTC_DIRSET |= PC_setup_gm;
	PORTD_DIRSET |= PD_setup_gm;
	PORTE_DIRSET |= PE_setup_gm;
	PORTF_DIRSET &= PF_setup_gm;
	
	PORTA_PIN7CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN6CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN5CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN4CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN3CTRL |= PORT_PULLUPEN_bm;
	PORTF_PIN3CTRL |= PORT_PULLUPEN_bm;

	PORTD_OUT = ~BARGRAPH_gm;
	PORTE_OUT &= ~(TIP_bm | PASS_bm | FAIL_bm);
	_delay_ms(1000);
	PORTD_OUT |= BARGRAPH_gm;
	PORTE_OUT |= TIP_bm | PASS_bm | FAIL_bm;


    while (1) 
    {
		if ((PORTF_IN & START_PB_bm) != 0) continue;
		
		PORTD_OUT = BARGRAPH_gm;

		PORTE_OUT = ~TIP_bm;

		gate_type = ~(PORTA_IN | 0x1F) >> 5 & 0x07;
		
		PORTD_OUT &= ~(gate_type & BARGRAPH_gm);

		if (gate_type == 4) {
			//enable pullups
			PORTA_PIN4CTRL |= PORT_PULLUPEN_bm;
			PORTA_PIN3CTRL |= PORT_PULLUPEN_bm;
			PORTB_PIN5CTRL |= PORT_PULLUPEN_bm;
			PORTB_PIN4CTRL |= PORT_PULLUPEN_bm;
			test();
			PORTA_PIN4CTRL &= ~PORT_PULLUPEN_bm;
			PORTA_PIN3CTRL &= ~PORT_PULLUPEN_bm;
			PORTB_PIN5CTRL &= ~PORT_PULLUPEN_bm;
			PORTB_PIN4CTRL &= ~PORT_PULLUPEN_bm;
		} else {
			test();
		}
		
    }
}

