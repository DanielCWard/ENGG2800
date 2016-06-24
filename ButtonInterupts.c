/*
 * ButtonInterupts.c
 *
 * Created: 22/10/2015 10:35:59 AM
 *  Author: Dan
 */ 
#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "sounds.h"
#include "USART.h"
#include "SPI.h"
#include "Buttons.h"
#include "ADC.h"
#include "PowerManagment.h"
#include "4PoleSwitch.h"
#include "I2C.h"

/*Turns on pin toggle interrupts for the 8 piano keys as a wake up method*/
void init_button_interrupts(void)
{
	//Enable buttons interrupts for PortA0 to A3 and B0 - B3
	PCICR |= (1 << PCIE0) | (1 << PCIE1);
	//Clear interrupt flag
	PCIFR |= (1 << PCIF0) | (1 << PCIF2);
	//Select A0-A3
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
	//Select B0-B3
	PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);
}

/*Disables the Pin toggles to resume use of the piano Keys as piano Keys*/
void disable_button_interrupts(void)
{
	//Disable buttons interrupts for PortA0 to A3 and B0 - B3
	PCICR &= ~((1 << PCIE0) | (1 << PCIE1));
}

/*ISR for the pin change interrupt for waking the avr on pins A0-A3*/
ISR(PCINT0_vect)
{
	disable_button_interrupts();
	turn_off_wdt();
	//Turn on Power LED
	PORTA |= (1 << PORTA5);
	sleep_disable();
	reload_all();
}

/*ISR for the pin change interrupt for waking the avr on pins B0-B3*/
ISR(PCINT1_vect)
{
	disable_button_interrupts();
	turn_off_wdt();
	//Turn On Power LED
	PORTA |= (1 << PORTA5);
	sleep_disable();
	reload_all();
}