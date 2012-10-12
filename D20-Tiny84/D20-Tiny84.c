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
	// Determine which Pin fired the interrupt
	cli();
	if (PINA & BIT(Button_Pin))	// Roll the Dice
	{
		displayRolling();
		displayNumber(dice);
	}
	sei();
}

int main(void)
{
	// config all functions and pinouts
	config();	
	dice = 20;
	
    while(1)
	{
		//displayRolling();
		//displayNumber(dice);
	}
}



