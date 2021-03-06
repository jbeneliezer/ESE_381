/*
 * MCP23S17_inout_test.c
 *
 * Created: 4/2/2021 4:28:58 PM
 * Author : Judah Ben-Eliezer
 */ 

#define F_CPU 4000000UL
#define IOCONaddr_b0 0x0A
#define IOCONaddr_b1 0x05
#define IODIRAaddr_b1 0x00
#define IODIRBaddr_b1 0x10
#define GPPUAaddr_b1 0x06
#define GPIOAaddr_b1 0x09
#define OLATBaddr_b1 0x1A
#define WRITE_opcode 0x40
#define READ_opcode 0x41

#include <avr/io.h>

void MCP23S17_SPI_init();
void MCP23S17_SPI_write(uint8_t, uint8_t, uint8_t);
uint8_t MCP23S17_SPI_read(uint8_t);

int main(void)
{
	PORTA.DIRSET = PIN4_bm | PIN6_bm | PIN7_bm;
	PORTA.DIRCLR = PIN5_bm;
	PORTF.DIRSET = PIN2_bm;
	
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc;
	PORTA.OUT &= ~PIN7_bm;

	volatile uint8_t i;

	MCP23S17_SPI_init();
	
	while (1)
	{
		i = MCP23S17_SPI_read(GPIOAaddr_b1);
		MCP23S17_SPI_write(WRITE_opcode, OLATBaddr_b1, i);
	}
}

void MCP23S17_SPI_init()
{
	MCP23S17_SPI_write(WRITE_opcode, IOCONaddr_b0, 0x80);
	MCP23S17_SPI_write(WRITE_opcode, IODIRAaddr_b1, 0xFF);
	MCP23S17_SPI_write(WRITE_opcode, IODIRBaddr_b1, 0x00);
	MCP23S17_SPI_write(WRITE_opcode, GPPUAaddr_b1, 0xFF);
}

void MCP23S17_SPI_write(uint8_t opcode, uint8_t addr, uint8_t data)
{
	PORTF.OUT &= ~PIN2_bm;
	SPI0.DATA = opcode;
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	SPI0.DATA = addr;
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	SPI0.DATA = data;
	while ((SPI0.INTFLAGS & PIN7_bm) == 0x00) {}
	PORTF.OUT |= PIN2_bm;
}

uint8_t MCP23S17_SPI_read(uint8_t addr)
{
	MCP23S17_SPI_write(READ_opcode, addr, 0x00);
	return SPI0.DATA;
}
