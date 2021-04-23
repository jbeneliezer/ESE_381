/*
 * MCP23S17_inout_interrupt_test.c
 *
 * Created: 4/2/2021 4:30:41 PM
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
#define GPINTENA 0x02

#include <avr/io.h>
#include <avr/interrupt.h>

void MCP23S17_SPI_init();
void MCP23S17_SPI_write(uint8_t, uint8_t, uint8_t);

volatile uint8_t i;

int main(void)
{
	PORTA.DIRSET = PIN4_bm | PIN6_bm | PIN7_bm;
	PORTA.DIRCLR = PIN5_bm;
	PORTF.DIRSET = PIN2_bm;
	PORTF.DIRCLR = PIN3_bm;
	PORTF.PIN3CTRL = PORT_ISC_LEVEL_gc;
	
	SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc;
	
	PORTA.OUT &= ~PIN7_bm;
	PORTF.OUT |= PIN2_bm;
	
	MCP23S17_SPI_init();
	
	sei();
	
	while (1)
	{
		__asm("nop");
	}
}


ISR(PORTF_PORT_vect) {
	MCP23S17_SPI_write(READ_opcode, GPIOAaddr_b1, 0x00);
	i = SPI0.DATA;
	MCP23S17_SPI_write(WRITE_opcode, OLATBaddr_b1, i);
	PORTF.INTFLAGS |= PIN3_bm;
}

void MCP23S17_SPI_init()
{
	MCP23S17_SPI_write(WRITE_opcode, IOCONaddr_b0, 0x80);
	MCP23S17_SPI_write(WRITE_opcode, IODIRAaddr_b1, 0xFF);
	MCP23S17_SPI_write(WRITE_opcode, IODIRBaddr_b1, 0x00);
	MCP23S17_SPI_write(WRITE_opcode, GPPUAaddr_b1, 0xFF);
	MCP23S17_SPI_write(WRITE_opcode, GPINTENA, 0xFF);
	
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
