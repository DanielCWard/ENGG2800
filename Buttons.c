/*
 * Buttons.c
 *
 * Created: 27/09/2015 9:37:05 AM
 *  Author: Dan
 */ 

#define F_CPU 20000000UL

#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

/*Global array which holds the Tuning word associated with the note assigned
 *to each of the buttons. Assigns left to right, left most button has note 
 *index 0. Default starts in lowest octave, unless EEPROM load changes that
 *each button is assigned an 8 bit value: the index to the note it is 
 *programmed to between 0-83*/
volatile uint8_t buttons[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	
//EEPROM MEMORY signature
static const uint8_t sig = 42;
uint8_t signature;

//Boolean: 1 if buttons in memory
uint8_t buttonsStored = 0;

/*store the 8 buttons at addresses:
button  | address
0		|	0
1		|	2
2		|	4
3		|	6
4		|	8
5		|	10
6		|	12
7		|	14
*/
//Saves button 0
void save_button_0(void)
{
	eeprom_update_byte((uint8_t*)(0), buttons[0]);
}

//Saves button 1
void save_button_1(void)
{
	eeprom_update_byte((uint8_t*)(2), buttons[1]);
}

//Saves button 2
void save_button_2(void)
{
	eeprom_update_byte((uint8_t*)(4), buttons[2]);
}

//Saves button 3
void save_button_3(void)
{
	eeprom_update_byte((uint8_t*)(6), buttons[3]);
}

//Saves button 4
void save_button_4(void)
{
	eeprom_update_byte((uint8_t*)(8), buttons[4]);
}

//Saves button 5
void save_button_5(void)
{
	eeprom_update_byte((uint8_t*)(10), buttons[5]);
}

//Saves button 6
void save_button_6(void)
{
	eeprom_update_byte((uint8_t*)(12), buttons[6]);
}

//Saves button 7
void save_button_7(void)
{
	//Save signature at address 100
	signature = sig;
	eeprom_update_byte((uint8_t*)100, signature);
	//Set and save buttonsStored
	buttonsStored = 1;
	eeprom_update_byte((uint8_t*)101, buttonsStored);
	eeprom_update_byte((uint8_t*)(14), buttons[7]);
}

/*Saves all Buttons array to EEPROM Memory*/
void save_buttons(void)
{
	//Save signature at address 100
	signature = sig;
	eeprom_update_byte((uint8_t*)100, signature);
	//Set and save buttonsStored
	buttonsStored = 1;
	eeprom_update_byte((uint8_t*)101, buttonsStored);
	//Save the button configuration in button/index order
	save_button_0();
	save_button_1();
	save_button_2();
	save_button_3();
	save_button_4();
	save_button_5();
	save_button_6();
	save_button_7();
}

/*Loads the buttons array from EEPROM*/
void load_buttons(void)
{
	//Read signature and buttonsStored
	signature = eeprom_read_byte((uint8_t*)100);
	buttonsStored = eeprom_read_byte((uint8_t*)101);
	
	if ((signature == sig) && buttonsStored) {
		//Buttons configuration is there: load it
		buttons[0] = eeprom_read_byte((uint8_t*)0);
		buttons[1] = eeprom_read_byte((uint8_t*)2);
		buttons[2] = eeprom_read_byte((uint8_t*)4);
		buttons[3] = eeprom_read_byte((uint8_t*)6);
		buttons[4] = eeprom_read_byte((uint8_t*)8);
		buttons[5] = eeprom_read_byte((uint8_t*)10);
		buttons[6] = eeprom_read_byte((uint8_t*)12);
		buttons[7] = eeprom_read_byte((uint8_t*)14);
	} //Else: default button frequencies will be used
}

/*Initializes the buttons: Data direction and loads config from memory*/
void init_buttons(void)
{
	//Set input DDR for (Button1-Button4: PORTA0-PORTA3, 
	//Button5 - Button8: PORTB0 - PORTB3)
	DDRA &= ~((1 << DDRA0) | (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3));
	DDRB &= ~((1 << DDRB0) | (1 << DDRB1) | (1 << DDRB2) | (1 << DDRB3));
	
	load_buttons();
}

/*Sets the given button to the value and then saves it to EEPROM*/
void set_save_button(uint8_t buttonNum, uint8_t buttonVal)
{
	switch (buttonNum) {
		case 0:
		buttons[0] = buttonVal;
		save_button_0();
		break;
		case 1:
		buttons[1] = buttonVal;
		save_button_1();
		break;
		case 2:
		buttons[2] = buttonVal;
		save_button_2();
		break;
		case 3:
		buttons[3] = buttonVal;
		save_button_3();
		break;
		case 4:
		buttons[4] = buttonVal;
		save_button_4();
		break;
		case 5:
		buttons[5] = buttonVal;
		save_button_5();
		break;
		case 6:
		buttons[6] = buttonVal;
		save_button_6();
		break;
		case 7:
		buttons[7] = buttonVal;
		save_button_7();
		break;
	}
}

/*Returns the 8bit value assigned to the given button, for buttons 0 - 3*/
uint8_t get_lower_button_tword(uint8_t buttonNum)
{
	switch (buttonNum) {
		case 0x01:
		//Button 1
		return buttons[0];
		case 0x02:
		//Button 2
		return buttons[1];
		case 0x04:
		//Button 3
		return buttons[2];
		case 0x08:
		//Button 4
		return buttons[3];
		case 0x10:
		//Button 1
		return buttons[4];
		case 0x20:
		//Button 2
		return buttons[5];
		case 0x40:
		//Button 3
		return buttons[6];
		case 0x80:
		//Button 4
		return buttons[7];
	}
	return 1;
}

/*Returns the 8bit value assigned to the given button, for buttons 4 - 7*/
uint8_t get_upper_button_tword(uint8_t buttonNum)
{
	switch (buttonNum) {
		case 0x01:
		//Button 1
		return buttons[4];
		case 0x02:
		//Button 2
		return buttons[5];
		case 0x04:
		//Button 3
		return buttons[6];
		case 0x08:
		//Button 4
		return buttons[7];
	}
	return 1;
}