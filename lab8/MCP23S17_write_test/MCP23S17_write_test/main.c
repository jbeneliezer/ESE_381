/*
 * MCP23S17_write_test.c
 *
 * Created: 4/1/2021 4:11:40 PM
 * Author : Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL

#include <avr/io.h>


void MCP23S17_SPI_init();
void MCP23S17_SPI_write(uint8_t, uint8_t, uint8_t);

int main(void)
{
	uint8_t i = 0x00;

	MCP23S17_SPI_init();
	
    while (1) 
    {
		if (i == 0xFF) {
			i = 0x00;	
		} else {
			++i;
		}
		MCP23S17_SPI_write(0,0,i);
    }
}

void MCP23S17_SPI_init() {
	PORTMUX.SPIROUTEA |= PORTMUX_SPI00_bm;
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc;
	PORTC.DIRSET = PIN0_bm | PIN2_bm | PIN3_bm;
	PORTC.DIRCLR = PIN1_bm;
	PORTF.DIRSET = PIN2_bm;
}

void MCP23S17_SPI_write(uint8_t opcode, uint8_t addr, uint8_t data) {
	
	while (SPI0.INTFLAGS & PIN7_bm == PIN7_bm) {}
	SPI0.DATA = data;
}
