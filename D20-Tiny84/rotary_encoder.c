/*
 * rotary_encoder.c
 *
 * Created: 12/10/2012 2:50:43 PM
 *  Author: Todd
 */ 

#include "d20.h"

static const int8_t enc_states [] PROGMEM = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; //encoder lookup table

// determine which direction the encoder was moved
void encoder_check()
{
	static uint8_t old_AB = 3; //lookup table index and initial state
	uint8_t encport; //encoder port copy
	uint8_t direction;
	
	old_AB <<=2;	 //remember previous state
	encport = CONTROL_PIN & 0x03;	//read encoder
	old_AB |= encport;	 //create index
	direction = pgm_read_byte(&(enc_states[( old_AB & 0x0f )]));	//get direction
	if( direction && ( encport == 3 )) {	//check if at detent and transition is valid
		if( direction == 1 ) {
			increment_dice();
		}
		else {
			decrement_dice();
		}
	}
}

// increment the dice index, rolling around to 0 if over max
void increment_dice()
{
	if (dice_index+1 >5) dice_index = 0;
	else dice_index += 1;
		
}

// decrement the dice index, rolling back to 5 if under min
void decrement_dice()
{
	if(dice_index-1<0) dice_index = 5;
	else dice_index -= 1;
}