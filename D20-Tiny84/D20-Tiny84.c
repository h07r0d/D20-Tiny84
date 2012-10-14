/*
 * D20_Tiny84.c
 *
 * Created: 11/10/2012 10:32:45 AM
 *  Author: Todd
 */ 

#include "d20.h"

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
		if (seed < 2147483647)	seed += 1;	//INT_MAX
		else seed = 0;
		eeprom_write_word((uint16_t*)E_SEED, seed);
	}
	
	// set last_vector to high for defaults
	last_vector = 0xFF;
	
	// set DATA, LATCH and CLOCK pins to OUTPUT for the bit shifter control
	CONTROL_DDR = 0;	// start off with everything as inputs
	CONTROL_DDR |= BIT(Data_Pin) | BIT(Latch_Pin) | BIT(Clock_Pin);
	
	// set Button and Encoder pin pull-ups
	CONTROL_PORT |= BIT(Button_Pin) | BIT(Encoder_A_Pin) | BIT(Encoder_B_Pin);
	
	// configure pin change interrupts for push button and encoder
	PCMSK0 |= BIT(Button_Pin) | BIT(Encoder_B_Pin) | BIT(Encoder_A_Pin);
	GIMSK |= BIT(PCIE0);

	// enable interrupts
	sei();
	
}

ISR(PCINT0_vect)
{
	cli();
	uint8_t _changed;
	_changed = CONTROL_PIN ^ last_vector;
	last_vector = CONTROL_PIN;
	
	if (_changed & BIT(Button_Pin))	// Roll the Dice
	{
		displayRolling();
		displayNumber(roll_dice());
	}
	else if (
		(_changed & BIT(Encoder_A_Pin)) || 
		(_changed & BIT(Encoder_B_Pin)))
	{
		encoder_check();	// increments or decrements dice_index accordingly
		displayDieSize();	// show what die size is going to be rolled next
	}
	sei();
}

// generate a 'rolled' value
uint16_t roll_dice(void)
{
	// mod against _dice to get the correct size
	uint8_t _dice = pgm_read_byte(&(dice_sizes[dice_index]));
	return (rnd16() % _dice) + 1;
}

int main(void)
{
	// config all functions and pinouts
	config();	
	dice_index = 5;	// 5th element in dice_sizes is 20
	displayDieSize();
	
    while(1)
	{
		//displayRolling();
		//displayNumber();
	}
}



