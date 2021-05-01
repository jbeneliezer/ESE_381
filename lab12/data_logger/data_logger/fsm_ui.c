/*
 * fsm_ui.c
 *
 * Created: 4/29/2021 5:41:27 PM
 *  Author: Judah Ben-Eliezer
 */

#define SEC_addr 0x00
#define MIN_addr 0x01
#define HOUR_addr 0x02

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "fsm_ui.h"
#include "DS1306_RTC_drivers.h"
#include "DOGM163W_A_lcd.h"
#include "adc.h"

uint8_t digits = 0;
dig = 0;

char* bcd2int(uint8_t bcd)
{
	char data[] = {(char) ((bcd & 0xF0) >> 4), (char) (bcd & 0x0F)};
	return data;
}

uint8_t int2bcd(int data)
{
	return (((data / 10) << 4) & 0xF0) | ((data % 10) & 0x0F);
}
	
void set_fn() {}

void hours_fn()
{
	/* print current hour */	
	write_line(3, strncat((char*) "h = ", bcd2int(read_RTC(HOUR_addr) | 0x3F), 6));
}

void minutes_fn()
{
	/* print current minute */
	uint8_t data = read_RTC(MIN_addr);
	write_line(3, strncat((char*) "m = ", bcd2int(read_RTC(MIN_addr)), 8));
}

void seconds_fn()
{
	/* print current second */
	write_line(3, strncat("s = ", bcd2int(read_RTC(SEC_addr)), 8));
}

void digit_fn()
{
	/* read digits into buffer */
	digits = digits * 10 + dig;
}

void enter_hours_fn()
{
	/* write new value for hours */
	write_RTC(HOUR_addr, PIN6_bm | int2bcd(digits % 24));
}

void enter_minutes_fn()
{
	/* write new value for minutes */
	write_RTC(MIN_addr, int2bcd(digits % 60));
	dig = 0;
}

void enter_seconds_fn()
{
	/* write new value for seconds */
	write_RTC(SEC_addr, int2bcd(digits % 60));
}

void error_fn()
{
	/* output error message */
	write_line(1, "");
	write_line(2, "invalid command.");
	write_line(3, "");
	_delay_ms(1000);
	dig = 0;
}


const transition idle_transitions[] =	//subtable for idle transitions.
{
	//input			next_state			task
	{s,				set,				set_fn},
	{h,				idle,				error_fn},
	{m,				idle,				error_fn},
	{e,				idle,				error_fn},
	{digit,			idle,				error_fn},
	{enter,			idle,				error_fn},
	{eol,			idle,				error_fn}
};

const transition set_transitions[] =	//subtable for set transitions.
{
	//input			next_state			task
	{s,				set,				set_fn},
	{h,				hours,				hours_fn},
	{m,				minutes,			minutes_fn},
	{e,				seconds,			seconds_fn},
	{digit,			idle,				error_fn},
	{enter,			idle,				error_fn},
	{eol,			idle,				error_fn}
};

const transition hour_transitions[] =	//subtable for hour transitions.
{
	//input			next_state			task
	{s,				set,				set_fn},
	{h,				idle,				error_fn},
	{m,				idle,				error_fn},
	{e,				idle,				error_fn},
	{digit,			digit,				digit_fn},
	{enter,			idle,				enter_hours_fn},
	{eol,			idle,				error_fn}
};

const transition minute_transitions[] =	//subtable for minute transitions.
{
	//input			next_state			task
	{s,				set,				set_fn},
	{h,				idle,				error_fn},
	{m,				idle,				error_fn},
	{e,				idle,				error_fn},
	{digit,			digit,				digit_fn},
	{enter,			idle,				enter_minutes_fn},
	{eol,			idle,				error_fn}
};

const transition second_transitions[] =	//subtable for second transitions.
{
	//input			next_state			task
	{s,				set,				set_fn},
	{h,				idle,				error_fn},
	{m,				idle,				error_fn},
	{e,				idle,				error_fn},
	{digit,			digit,				digit_fn},
	{enter,			idle,				enter_seconds_fn},
	{eol,			idle,				error_fn}
};

const transition* transitions[5] = // table of all transitions.
{
	idle_transitions,
	set_transitions,
	hour_transitions,
	minute_transitions,
	second_transitions
};


//***************************************************************************
// Function : state fsm_ui(state ps, key keyval)
// Date and version : 04/30/2021, version 1.0
// Target MCU : AVR128DB48 @ 4MHz
// Author : Judah Ben-Eliezer
// DESCRIPTION
// this function controls the finite state machine that describes the states
// of the data logger.
//**************************************************************************

state fsm_ui(state ps, key keyval)
{
	int i = 0;
	for (; (transitions[ps][i].keyval != keyval) && (transitions[ps][i].keyval != eol); ++i);
	
	transitions[ps][i].tf_ptr();
	
	return transitions[ps][i].next_state;
}
