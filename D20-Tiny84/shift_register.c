/*
 * shift_register.c
 *
 * Created: 11/10/2012 8:41:46 PM
 *  Author: Todd
 */ 
#include "d20.h"

// quick calls for SPI functions
void reset_SR(void)
{
	// Toggle Reset pin to clear Shift Registers (low/high)
	CONTROL_PORT &= ~_BV(Reset_Pin);
	CONTROL_PORT |= _BV(Reset_Pin);
}

void latch_in(void)
{
	// Toggle Latch Pin to trigger output from SR(high/low)
	CONTROL_PORT |= _BV(Latch_Pin);
	CONTROL_PORT &= ~_BV(Latch_Pin);
}

void shift_in(void)
{
	// Shift current bit into SR (high/low)
	CONTROL_PORT |= _BV(Clock_Pin);
	CONTROL_PORT &= ~_BV(Clock_Pin);
}


void shiftOut(uint16_t _number)
{
	uint8_t i;
	for (i=0;i<16;i++)
	{
		// a 1 should drive high, as a drive low turns on a segment
		if (bit_get(_number,BIT(i))) { CONTROL_PORT |= (1<< Data_Pin); }
		else { CONTROL_PORT &= ~(1<< Data_Pin); }

		// shift the bit into the SR
		shift_in();
	}
	
	// toggle latch to push bits to output pins
	latch_in();
}