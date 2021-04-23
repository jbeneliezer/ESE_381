/*
 * DS1306_RTC_drivers.h
 *
 * Created: 4/23/2021 11:34:34 AM
 *  Author: Judah Ben-Eliezer
 */ 


#ifndef DS1306_RTC_drivers_H_
#define DS1306_RTC_drivers_H_

void write_RTC (unsigned char reg_RTC, unsigned char data_RTC);
unsigned char read_RTC (unsigned char reg_RTC);
void SPI_rtc_ds1306_config(void);
void write_read_RTC_test(void);

#endif /* DS1306_RTC_drivers_H_ */