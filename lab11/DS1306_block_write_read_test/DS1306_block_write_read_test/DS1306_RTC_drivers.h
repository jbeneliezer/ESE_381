/*
 * DS1306_RTC_drivers.h
 *
 * Created: 4/23/2021 12:41:21 PM
 *  Author: Judah Ben-Eliezer
 */ 


#ifndef DS1306_RTC_DRIVERS_H_
#define DS1306_RTC_DRIVERS_H_

void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);
void SPI_rtc_ds1306_config(void);
void write_read_RTC_test(void);
void block_write_RTC (volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count)
void block_read_RTC (volatile unsigned char *array_ptr, unsigned char strt_addr, unsigned char count)
void block_write_read_RTC_test(void)

volatile unsigned char RTC_time_date_write[7];
volatile unsigned char RTC_time_date_read[7];
#endif /* DS1306_RTC_DRIVERS_H_ */