/*
 * DOGM163W_A_lcd.h
 *
 * Created: 4/30/2021 10:27:14 AM
 *  Author: Judah Ben-Eliezer
*/ 


#ifndef DOGM163W_A_LCD_H_
#define DOGM163W_A_LCD_H_

void lcd_spi_transmit_CMD (unsigned char);
void lcd_spi_transmit_DATA (unsigned char);
void init_spi_lcd (void);
void init_lcd_dog (void);
void delay_40mS(void);
void delay_30uS(void);
void update_lcd_dog(void);
void write_line(int, char*);

#endif /* DOGM163W_A_LCD_H_ */