/*
 * DOGM163W_A_lcd.c
 *
 * Created: 4/30/2021 10:25:39 AM
 *  Author: Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "DOGM163W_A_lcd.h"

// Display buffer for DOG LCD using sprintf()
char dsp_buff1[17];
char dsp_buff2[17];
char dsp_buff3[17];

///////////////////////// Driver Functions //////////////////////////

void lcd_spi_transmit_CMD (unsigned char cmd) {
	PORTC.OUT &= ~PIN0_bm;	// RS = 0 for cmd
	PORTF.OUT &= ~PIN3_bm;	// SS
	SPI0.DATA = cmd;		//send command
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	PORTF.OUT |= PIN3_bm;	// turn off SS
}

void lcd_spi_transmit_DATA (unsigned char cmd) {
	PORTC.OUT |= PIN0_bm;	// RS = 1 for data
	PORTF.OUT &= ~PIN3_bm;	// SS
	SPI0.DATA = cmd;		//send command
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	PORTF.OUT |= PIN3_bm;	// turn off SS
}

void init_spi_lcd (void) {
	
	PORTC.DIR |= PIN0_bm;	// RS
	PORTF.DIR |= PIN3_bm;	// SS
	PORTA.DIR = (PIN4_bm & ~PIN5_bm) | PIN6_bm;
	SPI0.CTRLB = SPI_SSD_bm | SPI_MODE_3_gc;
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
		
	PORTF.OUT |= PIN3_bm;
	PORTC.OUT &= ~PIN0_bm;	// RS = 0 for command
}


void init_lcd_dog (void) {
	
	init_spi_lcd();		//Initialize mcu for LCD SPI
	
	//start_dly_40ms:
	delay_40mS();    //startup delay.

	//func_set1:
	lcd_spi_transmit_CMD(0x39);   // send function set #1
	delay_30uS();	//delay for command to be processed


	//func_set2:
	lcd_spi_transmit_CMD(0x39);	//send fuction set #2
	delay_30uS();	//delay for command to be processed


	//bias_set:
	lcd_spi_transmit_CMD(0x1E);	//set bias value.
	delay_30uS();	//delay for command to be processed


	//power_ctrl:
	lcd_spi_transmit_CMD(0x55);	//~ 0x50 nominal for 5V
	//~ 0x55 for 3.3V (delicate adjustment).
	delay_30uS();	//delay for command to be processed


	//follower_ctrl:
	lcd_spi_transmit_CMD(0x6C);	//follower mode on...
	delay_40mS();	//delay for command to be processed
	delay_40mS();	//delay for command to be processed
	delay_40mS();	//delay for command to be processed
	delay_40mS();	//delay for command to be processed
	delay_40mS();	//delay for command to be processed


	//contrast_set:
	lcd_spi_transmit_CMD(0x7F);	//~ 77 for 5V, ~ 7F for 3.3V
	delay_30uS();	//delay for command to be processed


	//display_on:
	lcd_spi_transmit_CMD(0x0c);	//display on, cursor off, blink off
	delay_30uS();	//delay for command to be processed


	//clr_display:
	lcd_spi_transmit_CMD(0x01);	//clear display, cursor home
	delay_30uS();	//delay for command to be processed


	//entry_mode:
	lcd_spi_transmit_CMD(0x06);	//clear display, cursor home
	delay_30uS();	//delay for command to be processed
}

void delay_40mS(void) {
	int i;
	for (int n = 40; n > 0; n--)
	for (i = 0; i < 800; i++)
	__asm("nop");
}

void delay_30uS(void) {
	int i;
	for (int n = 1; n > 0; n--)
	for (i = 0; i < 2; i++)
	__asm("nop");
}

// Updates the LCD display lines 1, 2, and 3, using the
// contents of dsp_buff_1, dsp_buff_2, and dsp_buff_3, respectively.
void update_lcd_dog(void) {

	init_lcd_dog();
		
	// send line 1 to the LCD module.
	lcd_spi_transmit_CMD(0x80);	//init DDRAM addr-ctr
	delay_30uS();
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff1[i]);
		delay_30uS();
	}
	
	// send line 2 to the LCD module.
	lcd_spi_transmit_CMD(0x90);	//init DDRAM addr-ctr
	delay_30uS();
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff2[i]);
		delay_30uS();
	}
	
	// send line 3 to the LCD module.
	lcd_spi_transmit_CMD(0xA0);	//init DDRAM addr-ctr
	delay_30uS();
	for (int i = 0; i < 16; i++) {
		lcd_spi_transmit_DATA(dsp_buff3[i]);
		delay_30uS();
	}
}

void write_line(int line, char* message)
{
	switch (line)
	{
		case 1:
			strncpy(dsp_buff1, message, strlen(message) * sizeof(char*));
			break;
		case 2:
			strncpy(dsp_buff2, message, strlen(message) * sizeof(char*));
			break;
		case 3:
			strncpy(dsp_buff3, message, strlen(message) * sizeof(char*));
			break;
		default:
			break;
	}
	
	update_lcd_dog();
}
