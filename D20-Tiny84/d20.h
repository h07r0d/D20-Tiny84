/*
 * d20.h
 *
 * Created: 11/10/2012 10:58:07 AM
 *  Author: Todd
 */ 


#ifndef D20_H_
#define D20_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/sfr_defs.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#define F_CPU 4000000	//4MHz
/*
 * Pin Definitions and bitwise helpers
 */
#define E_SEED		0
#define E_STORED		2
#define CONTROL_DDR		DDRA
#define CONTROL_PORT	PORTA
#define Button_Pin		PA6
#define Data_Pin		PA2
#define Latch_Pin		PA4
#define Clock_Pin		PA3
#define Reset_Pin		PB3
#define Encoder_A_Pin	PA0
#define Encoder_B_Pin	PA1
#define bit_set(p,m) ((p) |= (m))
#define bit_get(p,m) ((p) & (m))
#define BIT(x) (0x01 << (x))

// Function definitions
uint16_t rnd16(void);
uint16_t getBitPattern(uint16_t);
uint8_t getDigit(uint16_t, uint8_t);
uint8_t roll_button(void);
void shiftOut(uint16_t);
void config(void);
void reset_SR(void);
void shift_in(void);
void latch_in(void);
void displayRolling(void);
void displayNumber(uint8_t);

// Globals
volatile uint16_t seed;

#endif /* D20_H_ */