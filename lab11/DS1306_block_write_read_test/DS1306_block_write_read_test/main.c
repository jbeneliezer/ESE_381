/*
 * DS1306_block_write_read_test.c
 *
 * Created: 4/23/2021 12:40:44 PM
 * Author : Judah Ben-Eliezer
 */ 

#include <avr/io.h>


int main(void)
{

	SPI_rtc_ds1306_config();

	block_write_read_RTC_test();
	
    while (1) 
    {
		
    }
}

