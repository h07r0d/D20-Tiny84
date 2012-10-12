/*
 * display.c
 *
 * Created: 12/10/2012 2:10:44 PM
 *  Author: Todd
 */ 

#include "d20.h"
const uint8_t outputNumbers[] = {0x88, 0xEB, 0x4C, 0x49, 0x2B, 0x19, 0x18, 0xCB, 0x08, 0x09};
const uint8_t powers[] = {1,10};
volatile uint16_t lfsr16;
volatile uint16_t outBits = 0;
/*
 * Random Number generator.  Uses seed value from EEPROM if necessary.
 * Pulled from wikipedia as a Linear congruential generator
 */
uint16_t rnd16()
{
	if(lfsr16 == 0) lfsr16 = seed;
	lfsr16 = (lfsr16 >> 1) ^ (-(lfsr16 & 1u) & 0xB400u);
	return lfsr16;
}


/*
 * use random number to select digits for display
 * i.e. 09	- first digit is 0.  pull outputNumbers[0] and put that
 *			  bit pattern in the first 8 bits.
 *			- second digit is 9.  pull outputNumbers[9] and put that
 *			  bit pattern in the second 8 bits.
 */
uint16_t getBitPattern(uint16_t _number)
{
	uint16_t _value = 0x00;	
	_value |= (outputNumbers[getDigit(_number, 1)]) << 8;
	_value |= outputNumbers[getDigit(_number,0)];	
	return _value;
}

/*
 * get digit in given place in powers array
 * powers array contains all place holder digits
 */
uint8_t getDigit(uint16_t _number, uint8_t _digit)
{
	return ((_number / powers[_digit])%10);
}


// Display random garbage to simulate "rolling"
void displayRolling(void)
{
	uint16_t rng=0;
	uint8_t i;
	for(i=15;i>0;--i)
	{
		rng = rnd16();
		
		// cheating a bit.
		// random looks better without the decimal points
		// just mask them out
		bit_set(rng, BIT(3));
		bit_set(rng, BIT(11));
		shiftOut(rng);
#ifndef __DEBUG__
		_delay_ms(100);
#endif				
	}	
}

// Display 'Rolled' Number.  The _dice value determine the die size being rolled for
void displayNumber(uint8_t _dice)
{
	// mod against _dice to get the correct size
	uint16_t _rnd = (rnd16() % _dice) + 1;
	outBits = getBitPattern(_rnd);				
	reset_SR();		
		
#ifndef __DEBUG__
	shiftOut(0xFFFF);
	_delay_ms(100);
	shiftOut(outBits);	
	//flash results
	_delay_ms(200);
	shiftOut(0xFFFF);
	_delay_ms(200);
	shiftOut(outBits);
	//wait long enough to see results
	_delay_ms(2000);
#endif	
}

