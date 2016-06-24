/*
 * _4PoleSwitch.c
 *
 * Created: 9/10/2015 2:58:43 PM
 *  Author: Dan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sounds.h"

/*Sets up the Data direction and interrupt PCIE3 for the 4 pole switch
connected to:
<Pin>: <WaveType> (<Interrupt>)
PIND2: SAW Wave (PCINT26)
PIND3: Sine Wave (PCINT27)
PIND4: Square Wave (PCINT28)
PIND5: Triangle Wave (PCINT29)
*/
void init_switch(void)
{
	DDRD &= ~((1 << DDRD2) | (1 << DDRD3) | (1 << DDRD4) | 
	(1 << DDRD5));
	/*//Enable Pin toggle interrupts
	PCICR |= (1 << PCIE3);
	//Ensure interrupt flag is cleared
	PCIFR |= (1 << PCIF3);
	
	//Select pins D2 - D5
	PCMSK3 |= (1 << PCINT26) | (1 << PCINT27) | (1 << PCINT28) | 
	(1 << PCINT29);
	//Set initial waveform*/
	check_switch();
}

/*pin change Interrupt inside Sounds.c*/

/*Disables the pin change interrupt for the 4 pole switch to ensure 
 *it doesn't wake up
 *the AVR from the Sleep mode*/
void disable_switch(void)
{
	PCICR &= ~(1 << PCIE3);
}