/*
 * ic_test_ident.c
 *
 * Created: 3/4/2021 6:59:47 PM
 * Author : Judah Ben-Eliezer
 */

#define  F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>

#define IN_COMB_00 0x00															// input combination 00.
#define IN_COMB_01 0x55															// input combination 01.
#define IN_COMB_10 0xAA															// input combination 10.
#define IN_COMB_11 0xFF															// input combination 11.
#define DIP_SW_gm 0xE0															// group mask for DIP switches.
#define START_PB_bm 0x08														// bit mask for pushbutton.
#define BARGRAPH_gm 0x07														// group mask for bargraph.
#define TIP_bm 0x04																// bit mask for TIP LED.
#define PASS_bm 0x01															// bit mask for green LED.
#define FAIL_bm 0x02															// bit mask for red LED.
#define A_GATES_OUT_gm 0x1F														// group mask for DUT outputs to port A.
#define B_GATES_OUT_gm 0x30														// group mask for DUT outputs to port B.
#define PA_setup_gm 0x07														// group mask for port A inputs.
#define PB_setup_gm 0xCF														// group mask for port B inputs.
#define PC_setup_gm 0xFF														// group mask for port C outputs.
#define PD_setup_gm 0x07														// group mask for port D outputs.
#define PE_setup_gm 0x0F														// group mask for port E outputs.
#define PF_setup_gm 0xF7														// group mask for port F inputs.																												

const uint8_t stimulus[] = {IN_COMB_00, IN_COMB_01, IN_COMB_10, IN_COMB_11};	// array of test inputs for ic.
	
const uint8_t verify[5][4] = {{0x0F, 0x0F, 0x0F, 0x00},							// matrix of correct results for comparison to test outputs.
							 {0x00, 0x00, 0x00, 0x0F},
							 {0x00, 0x0F, 0x0F, 0x0F},
							 {0x00, 0x0F, 0x0F, 0x00},
							 {0x0F, 0x0F, 0x0F, 0x00}};
												
uint8_t gate_type;																// variable for type of gate specified by input.

uint8_t i, j;																	// global iterators for looping through test inputs and gate types.

/* this function test the DUT.
 * it outputs every possible input to the DUT and confirms the correct output.
 */
uint8_t test(uint8_t gate) {
	PORTE_OUT |= PIN3_bm;														// turn DUT pin 14 on.

	for (i = 0; i < 4; ++i) {
		PORTC_OUT = stimulus[i];
			
		_delay_loop_1(2);
			
		uint8_t a_in = (PORTA_IN & A_GATES_OUT_gm) >> 3 & 0x03;
		uint8_t b_in = (PORTB_IN & B_GATES_OUT_gm) >> 2 & 0x0C;

		if (!((a_in | b_in) == verify[gate][i])) break;
		
	}

	PORTE_OUT &= ~PIN3_bm;														// turn DUT pin 14 off.

	if (i == 4) return 1;
	else return 0;
}

/* this function what IC is being tested.
 * it does this by using the test function for each IC it knows.
 * return values:
 * 0 = 74HC00
 * 1 = 74HC08
 * 2 = 74HC32
 * 3 = 74HC86
 * 4 = 74LS03
 * 7 = unknown
 */
uint8_t identify() {

	for (j = 0; j < 4; ++j) {
		if (test(j) == 1) return j;
	}
	PORTA_PIN4CTRL |= PORT_PULLUPEN_bm;											// enable pullups.
	PORTA_PIN3CTRL |= PORT_PULLUPEN_bm;
	PORTB_PIN5CTRL |= PORT_PULLUPEN_bm;
	PORTB_PIN4CTRL |= PORT_PULLUPEN_bm;
	if (test(4) == 1)  {
		PORTA_PIN4CTRL &= ~PORT_PULLUPEN_bm;									// disable pullups.
		PORTA_PIN3CTRL &= ~PORT_PULLUPEN_bm;
		PORTB_PIN5CTRL &= ~PORT_PULLUPEN_bm;
		PORTB_PIN4CTRL &= ~PORT_PULLUPEN_bm;
		return 4;
	} else {
		PORTA_PIN4CTRL &= ~PORT_PULLUPEN_bm;									// disable pullups.
		PORTA_PIN3CTRL &= ~PORT_PULLUPEN_bm;
		PORTB_PIN5CTRL &= ~PORT_PULLUPEN_bm;
		PORTB_PIN4CTRL &= ~PORT_PULLUPEN_bm;
		return 7;
	}
}

int main(void)
{
	PORTA_DIRSET &= PA_setup_gm;												// configure input and output ports.
	PORTB_DIRSET &= PB_setup_gm;
	PORTC_DIRSET |= PC_setup_gm;
	PORTD_DIRSET |= PD_setup_gm;
	PORTE_DIRSET |= PE_setup_gm;
	PORTF_DIRSET &= PF_setup_gm;
	
	PORTA_PIN7CTRL |= PORT_PULLUPEN_bm;											// enable pullups.
	PORTA_PIN6CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN5CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN4CTRL |= PORT_PULLUPEN_bm;
	PORTA_PIN3CTRL |= PORT_PULLUPEN_bm;
	PORTF_PIN3CTRL |= PORT_PULLUPEN_bm;

	PORTD_OUT = ~BARGRAPH_gm;													// turn on all bargraph LEDs.
	PORTE_OUT &= ~(TIP_bm | PASS_bm | FAIL_bm);									// turn on all other LEDs.

	_delay_ms(1000);															// delay 1 second.

	PORTD_OUT |= BARGRAPH_gm;													// turn off all bargraph LEDs.
	PORTE_OUT |= TIP_bm | PASS_bm | FAIL_bm;									// turn off all other LEDs.

    while (1) 
    {
		if ((PORTF_IN & START_PB_bm) != 0) continue;							// check if pushbutton is pressed.
		
		PORTD_OUT = BARGRAPH_gm;												// turn off bargraph LEDs.

		PORTE_OUT = ~TIP_bm;													// turn on TIP LED.

		gate_type = ~(PORTA_IN | 0x1F) >> 5 & 0x07;								// get gate_type from DIP switch input.
		
		if (gate_type == 0x04) {												// enable pullups for test of 74LS03.
			PORTA_PIN4CTRL |= PORT_PULLUPEN_bm;						
			PORTA_PIN3CTRL |= PORT_PULLUPEN_bm;
			PORTB_PIN5CTRL |= PORT_PULLUPEN_bm;
			PORTB_PIN4CTRL |= PORT_PULLUPEN_bm;

			if (test(gate_type) == 1) PORTE_OUT &= ~PASS_bm;					// turn green LED on.
			else PORTE_OUT &= ~FAIL_bm;											// turn red LED on.

			PORTA_PIN4CTRL &= ~PORT_PULLUPEN_bm;
			PORTA_PIN3CTRL &= ~PORT_PULLUPEN_bm;
			PORTB_PIN5CTRL &= ~PORT_PULLUPEN_bm;
			PORTB_PIN4CTRL &= ~PORT_PULLUPEN_bm;
		} else if (gate_type == 0x07) {											// call identify function for input combination of 7.
			gate_type = identify();
		} else {
			if (test(gate_type) == 1) PORTE_OUT &= ~PASS_bm;					// turn green LED on.
			else PORTE_OUT &= ~FAIL_bm;											// turn red LED on.
		}

		PORTD_OUT = ~(gate_type & BARGRAPH_gm);									// output gate_type to bargraph LEDs.
		
		PORTE_OUT |= TIP_bm;													// turn TIP LED off.
		
		PORTC_OUT = stimulus[0];												// set DUT inputs to zero to prevent negative flow to pin 14.

    }
	
	return 0;
}

