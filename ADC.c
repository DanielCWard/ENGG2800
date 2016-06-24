/*
 * ADC.c
 *
 * Created: 29/09/2015 3:35:55 PM
 *  Author: Dan
 
 ADC uses VDD (+5V) connected to the AREF pin
 */ 
#define F_CPU 20000000UL

#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"
#include "I2C.h"
#include <util/twi.h>

/*Sets up the ADC*/
void init_adc(void)
{
	//Set ADC to complete only on PORTA7
	//Left adjust: Allows 8 bit resolution, 2LSB's are dropped
	//Use AREF pin
	ADMUX = (1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << ADLAR);
	
	//Enable ADC, Dont convert yet
	//CLock divider of 128 = ADC clk of 156250Hz
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/*Turns off the ADC for power saving mode*/
void disable_adc(void)
{
	ADCSRA = 0xFF;
}

/*Reads the manual pot on synth board and sends the value to control
*the I2C POT on VCF.
*The digital pot should be a AD5241, this function puts the set resistance
*out on PORTB
*of the POT and uses RDAC 1
*Writes to resistance between pin 1 and wiper (pin 2)
*range 0 - 255, 0 is max resistance*/
void get_POT_set_TWIPOT(void)
{
	static uint8_t result = 0;
	//Start conversion
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & ( 1 << ADSC)) {
		; //Wait
	}
	result = ADCH; //Drop 2 LSBs

	//Set the TWI POT
	//Address to POT with select pins to GND, specify Write to device
	i2c_send_byte_1(LOW, LOW, LOW);
	//enable output 1, enable output 2, RDAC 1 selected
	i2c_send_byte_2(HIGH, HIGH, LOW);
	i2c_send_byte_3(result);	// RDAC1 value 28  for variable resistor
	i2c_stop_byte();
}

/*starts the ADC conversion of the POT connected to PINA7*/
void start_ADC_conversion(void)
{
	ADCSRA |= (1 << ADSC);
}

/*Gets the result of the last ADC conversion if ready*/
void get_ADC_result(void)
{
	static uint8_t result = 0x00;
	if (ADCSRA & ( 1 << ADSC)) {
		;
	} else {
		result = ADC;
	}
	PORTD = result;
	//return result;
}

/*Sends the given 8bit value to the digital pot connected to i2c pins
 *The digital pot should be a AD5241, this function puts the set 
 *resistance out on PORTB of the POT and uses RDAc 1
 *Writes to resistance between pin 1 and wiper (pin 2)
 *range 0 - 255, 0 is max resistance*/
void set_POT(uint8_t val)
{
	//Address to POT with select pins to GND, specify Write to device
	i2c_send_byte_1(LOW, LOW, LOW);
	//enable output 1, enable output 2, RDAC 1 selected
	i2c_send_byte_2(HIGH, HIGH, LOW);
	i2c_send_byte_3(val);	// RDAC1 value 28  for variable resistor
	i2c_stop_byte();
}