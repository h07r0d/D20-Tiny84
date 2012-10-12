/*
 * D20_Tiny84.c
 *
 * Created: 11/10/2012 10:32:45 AM
 *  Author: Todd
 */ 

#include "d20.h"

// Bit array to display numbers 0-9
const uint8_t outputNumbers[] = {0x88, 0xEB, 0x4C, 0x49, 0x2B, 0x19, 0x18, 0xCB, 0x08, 0x09};
const uint8_t powers[] = {1,10};
volatile uint16_t lfsr16;
volatile uint16_t seed ;
volatile uint16_t outBits = 0;
volatile int threshold = 0;
uint8_t roll;


int main(void)
{
	// config all functions and pinouts
	config();	
			
    while(1)
	{
		//if(roll_button)
		//{
			displayRolling();
			displayNumber(20);
		//}
	}
}
/*
uint8_t roll_button()
{
	if (bit_is_clear(PINB, PB5))
	{
		_delay_ms(30);
		if (bit_is_clear(PINB,PB5)) return 1;
	}
	return 0;
}
*/
//ISR(PCINT0_vect)
//{
	// button pushed
	//roll = 1;
//}


void config(void)
{
	// Get seed value from EEPROM.  If first run, set value
	if(eeprom_read_byte((uint8_t*)E_STORED)!='S')
	{
		// non 'S' means first run, write seed value
		eeprom_write_word((uint16_t*)E_SEED,0x01);
		eeprom_write_byte((uint8_t*)E_STORED,'S');
	}
	else
	{
		seed = eeprom_read_word((uint16_t*)E_SEED);
		if (seed < 2147483647)	// INT_MAX
			seed += 1;
		else
			seed = 0;
		eeprom_write_word((uint16_t*)E_SEED, seed);
	}	
		
	
	// set DATA, LATCH and CLOCK pins to OUTPUT for the bit shifter control
	CONTROL_DDR = 0;	// start off with everything as inputs
	CONTROL_DDR |= BIT(Data_Pin) | BIT(Latch_Pin) | BIT(Clock_Pin);
	
	// set Button and Encoder pin pull-ups
	//CONTROL_PORT |= (1 << PB0) | (1 << PB1) | (1 << PB5);
	CONTROL_PORT |= BIT(Button_Pin) | BIT(Encoder_A_Pin) | BIT(Encoder_B_Pin);
	
	// configure pin change interrupts for push button
	//PCMSK |= (1 << PCINT5);
	//GIMSK |= (1 << PCIE) | (1 << INT0);
	//MCUCR |= (1 << ISC00);

	// enable interrupts
	//sei();
	
}
//
// use for random LED flashing
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
