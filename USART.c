/*
 * USART.c
 *
 * Created: 13/09/2015 1:33:19 PM
 *  Author: Dan
 */ 
#define F_CPU 20000000UL

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*Set up the USART Communication
Putty Settings:
>Find COM PORT
>Baud should match, default 2400
>8 data bits
>2 stop bits
>no parity
>flowcontrol: xon/xoff*/
void init_serial(uint16_t baud)
{
	//For the RX LED flashing
	//PORTC7 for RX LED
	DDRC |= (1 << DDRC7);
	
	//Put B0 low
	PORTB &= ~(1<<PORTB0);
	//For 20MHz, baud rate: 2400 offers 0% error
	uint16_t ubrr;
	//Set UBRR0 to given baud rate
	//Rounded by integer division
	//multiply by 2 for double speed
	ubrr = ((((F_CPU / (8 * baud)) + 1)/2 - 1)*2);
	//Set the register
	UBRR0 = ubrr;
	
	//Set double speed bit
	UCSR0A |= (1 << U2X0);
	
	//Enable receive and transmission
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	//Set data frame format: 8 bits, 2 stop bits
	UCSR0C = (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
	//Enable receive complete interrupt for when we get a message
	UCSR0B |= (1 << RXCIE0);
	//Interrupts should be turned on in main
}

/*Disables the USART for power saving mode*/
void disable_usart(void)
{
	UCSR0B = 0x00;
}

/*Receive complete interrupt handler is in Sounds.c
This is because it modified the global frequency variables*/

/*Turns on the usart receive LED on C7*/
void enable_RX_LED(void)
{
	PORTC |= (1 << PORTC7);
}

/*Turns off the USART Receive LED on C7*/
void disable_RX_LED(void)
{
	PORTC &= ~(1 <<PORTC7);
}

//For Debugging
//Send an 8 bit number to USART
void send_num(uint8_t number)
{
	if (number < 10) {
		UDR0 = (number + '0');
		_delay_ms(5);
		} else if (number < 100) {
		UDR0 = ((number / 10) + '0');
		_delay_ms(5);
		UDR0 = ((number % 10) + '0');
		} else {
		UDR0 = ((number / 100) + '0');
		number = (number % 100);
		_delay_ms(5);
		UDR0 = ((number / 10) + '0');
		_delay_ms(5);
		UDR0 = ((number % 10) + '0');
	}
	_delay_ms(5);
	UDR0 = '\r';
	_delay_ms(5);
	UDR0 = '\n';
}

/*Simple Echoer to debug, flashes B0 LED
ISR(USART0_RX_vect)
{
	//Flash RX LED on PORTC7
	enable_RX_LED();
	char input;
	input = UDR0;
	//Be a neg
	UDR0 = input;
	//So we can see the LED
	_delay_ms(1);
	
	//put LED back low
	disable_RX_LED();
}*/