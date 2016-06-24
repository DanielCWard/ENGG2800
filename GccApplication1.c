/*
 * GccApplication1.c
 *
 * Created: 6/09/2015 8:01:49 AM
 *  Author: Daniel Ward
 */ 


#define F_CPU 20000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sounds.h"
#include "USART.h"
#include <util/delay.h>
#include "SPI.h"
#include "Buttons.h"
#include "ADC.h"
#include "PowerManagment.h"
#include "4PoleSwitch.h"
#include "I2C.h"
#include "ButtonInterrupts.h"


int main(void)
{
	DDRC |= (1 << DDRC7);
	PORTC |= (1 << PORTC7);
	_delay_ms(2);
	PORTC &= ~(1 << PORTC7);
	
 	init_power();
 	init_timeout_timer();
	init_switch();
	init_ref_clock();
	init_spi(2);
	init_buttons();
	init_serial(2400);
	init_adc();
	init_i2c();
	//Turn on interrupts
	sei();
	play_sounds();
	//Debug: Never Gets here
    while(1)
    {
		_delay_ms(1000);
		get_POT_set_TWIPOT();
	}
}