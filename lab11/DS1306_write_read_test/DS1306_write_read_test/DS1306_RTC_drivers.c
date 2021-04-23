/*
 * DS1306_RTC_drivers.c
 *
 * Created: 4/23/2021 11:23:44 AM
 * Author : Judah Ben-Eliezer
 */ 

#define CTRL_REG_READ 0x0F
#define CTRL_REG_WRITE 0x8F
#define RAM_BEGIN 0x20
#define RAM_END 0x7F

#include <avr/io.h>
#include "DS1306_RTC_drivers.h"

//***************************************************************************
// Function : void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
// Date and version : 041221, version 1.0
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author : Judah Ben-Eliezer
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing the
// data (data_RTC). In the DS1306 data sheet this operation is called an SPI
// single-byte write.
//**************************************************************************
void write_RTC (unsigned char reg_RTC, unsigned char data_RTC)
{
	PORTC.OUT |= PIN1_bm;
	SPI0.DATA = 0x80 | reg_RTC;
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	SPI0.DATA = data_RTC;
	PORTC.OUT &= ~PIN1_bm;
}

//***************************************************************************
// Function Name : unsigned char read_RTC (unsigned char reg_RTC)
// Date and version : 041221, version 1.0
// Target MCU : AVR128DB48 @ 4MHz
// Author : Judah Ben-Eliezer
// DESCRIPTION
// This function reads data from a register in the RTC. To accomplish this, it
// must first write the register's address (reg_RTC) followed by writing a dummy
// byte to generate the SCLKs to read the data (data_RTC). In the DS1306 data
// sheet this operation is called am SPI single-byte read.
//**************************************************************************
unsigned char read_RTC (unsigned char reg_RTC)
{
	write_RTC(reg_RTC, 0x00);
	return SPI0.DATA;
}

//******************************************************************************
// Function : void SPI_rtc_ds1306_config (void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Judah Ben-Eliezer
// DESCRIPTION
// This function unselects the ds_1306 and configures an AVR128DB48 operated at
// 4 MHz to communicate with the ds1306. Pin PC1 of the AVR128DB48 is used to
// select the ds_1306. SCLK is operated a the maximum possible frequency for
// the ds1306.
//******************************************************************************
void SPI_rtc_ds1306_config(void)
{
	PORTC.DIR = PIN1_bm;
	PORTA.DIR = (PIN4_bm & ~PIN5_bm) | PIN6_bm;
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm;
	SPI0.CTRLB = SPI_SSD_bm | SPI_MODE1_bm;}


//***************************************************************************
// Function Name : void write_read_RTC_test(void)
// Date and version : 041221, version 1.0
// Target MCU : AVR128DB48 @ 4MHz
// Author : Judah Ben-Eliezer
// DESCRIPTION
// This function writes a byte to the NV RAM and then it reads back the location
// just written and places the result in a global array named RTC_byte[]. The
// function repeats this write/read sequence 10 times. The locations written are
// 0xA0 through 0xA9 and the corresponding locations read are 0x20 through 0x29.
//**************************************************************************
void write_read_RTC_test(void)
{
	uint8_t RTC_byte[10];
	uint8_t i = 0x00;
	for (; i < 0x0A; ++i)
	{
		write_RTC(RAM_BEGIN + i, 'U');
		RTC_byte[i] = read_RTC(RAM_BEGIN + i);
	}
}



