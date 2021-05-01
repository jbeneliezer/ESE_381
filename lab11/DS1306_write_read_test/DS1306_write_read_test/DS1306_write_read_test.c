/*
 * DS1306_write_read_test.c
 *
 * Created: 4/23/2021 11:35:04 AM
 *  Author: Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "DS1306_RTC_drivers.h"

int main(void)
{
	
	SPI_rtc_ds1306_config();
	
	while (1)
	{
		write_read_RTC_test();
		_delay_ms(10);
		
	}

	return 0;
}
