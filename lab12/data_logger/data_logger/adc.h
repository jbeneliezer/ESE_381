/*
 * adc.h
 *
 * Created: 4/30/2021 1:28:16 PM
 *  Author: Judah Ben-Eliezer
 */ 


#ifndef ADC_H_
#define ADC_H_

void adc_init(void);
int convert(uint16_t);
char* bin2ascii(int);



#endif /* ADC_H_ */