/*
 * sounds.c
 *
 * Created: 6/09/2015 8:02:15 AM
 *  Author: Daniel Ward
 */ 

#define F_CPU 20000000UL

#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "SPI.h"
#include "LUTs.h"
#include "ADC.h"
#include "Buttons.h"
#include "PowerManagment.h"
#include "USART.h"

/*Function Prototypes*/
void note_off(uint8_t noteIndex);

#define SAMPLE_NUM 255//size of LUT
#define REF_CLK 31250 //frequency of reference clock 31.25kHz
#define TOTAL_NOTES 84 //Total notes possible

//Global Wave From
volatile const uint8_t* waveForm;
volatile uint8_t check = 0;

//Global Buttons variables
static uint8_t buttonState1 = 0x00;
static uint8_t prevButtonState1 = 0x00;
static uint8_t buttonState2 = 0x00;
static uint8_t prevButtonState2 = 0x00;

//The buttons Array, see buttons.c
extern uint8_t buttons[8];

//Has switch changed
static uint8_t prevSwitchState = 0x00;
static uint8_t switchState = 0x00;

/*Global variables for waves*/

//Phase accumulators
volatile uint16_t phacc1 = 0;
volatile uint16_t phacc2 = 0;
volatile uint16_t phacc3 = 0;

//Step size, tuning words
volatile uint16_t tword1;
volatile uint16_t tword2;
volatile uint16_t tword3;
	
//Global Buffer for USART MESSAGES
uint8_t uartBuffer[10];
uint8_t uartCount = 0;

/*Struct for linked list*/
struct noteNode {
	uint8_t name;
	volatile struct noteNode* next;
	volatile struct noteNode* previous;
	};
	
/*Global Array of structs for the notes playing*/
volatile struct noteNode* noteNodes[TOTAL_NOTES + 3];
volatile struct noteNode* head;
volatile struct noteNode* tempHead;

/*Declare 1 node for each note*/
volatile static struct noteNode node0, node1, node2, node3, node4, node5,
	node6, node7, node8, node9, node10, node11, node12, node13, node14, node15,
	node16, node17, node18, node19, node20, node21, node22, node23, node24,
	node25, node26, node27, node28, node29, node30, node31, node32, node33,
	node34, node35, node36, node37, node38, node39, node40, node41, node42,
	node43, node44, node45, node46, node47, node48, node49, node50, node51,
	node52, node53, node54, node55, node56, node57, node58, node59, node60,
	node61, node62, node63, node64, node65, node66, node67, node68, node69,
	node70, node71, node72, node73, node74, node75, node76, node77, node78,
	node79, node80, node81, node82, node83, dummy1, dummy2, dummy3;

/*Global array, represents notes from index's 0 to 83, value at array index
 *is 0x00 if note is not being played and 0xFF*/
	volatile uint8_t notesOn[TOTAL_NOTES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0};

/*Populates noteNodes array with the nodes for each note
 * allowing the formulation of statically declared linked list*/
void populate_note_buffer(void)
{
	noteNodes[0] = &node0;
	noteNodes[0]->name = 0;
	noteNodes[1] = &node1;
	noteNodes[1]->name = 1;
	noteNodes[2] = &node2;
	noteNodes[2]->name = 2;
	noteNodes[3] = &node3;
	noteNodes[3]->name = 3;
	noteNodes[4] = &node4;
	noteNodes[4]->name = 4;
	noteNodes[5] = &node5;
	noteNodes[5]->name = 5;
	noteNodes[6] = &node6;
	noteNodes[6]->name = 6;
	noteNodes[7] = &node7;
	noteNodes[7]->name = 7;
	noteNodes[8] = &node8;
	noteNodes[8]->name = 8;
	noteNodes[9] = &node9;
	noteNodes[9]->name = 9;
	noteNodes[10] = &node10;
	noteNodes[10]->name = 10;
	noteNodes[11] = &node11;
	noteNodes[11]->name = 11;
	noteNodes[12] = &node12;
	noteNodes[12]->name = 12;
	noteNodes[13] = &node13;
	noteNodes[13]->name = 13;
	noteNodes[14] = &node14;
	noteNodes[14]->name = 14;
	noteNodes[15] = &node15;
	noteNodes[15]->name = 15;
	noteNodes[16] = &node16;
	noteNodes[16]->name = 16;
	noteNodes[17] = &node17;
	noteNodes[17]->name = 17;
	noteNodes[18] = &node18;
	noteNodes[18]->name = 18;
	noteNodes[19] = &node19;
	noteNodes[19]->name = 19;
	noteNodes[20] = &node20;
	noteNodes[20]->name = 20;
	noteNodes[21] = &node21;
	noteNodes[21]->name = 21;
	noteNodes[22] = &node22;
	noteNodes[22]->name = 22;
	noteNodes[23] = &node23;
	noteNodes[23]->name = 23;
	noteNodes[24] = &node24;
	noteNodes[24]->name = 24;
	noteNodes[25] = &node25;
	noteNodes[25]->name = 25;
	noteNodes[26] = &node26;
	noteNodes[26]->name = 26;
	noteNodes[27] = &node27;
	noteNodes[27]->name = 27;
	noteNodes[28] = &node28;
	noteNodes[28]->name = 28;
	noteNodes[29] = &node29;
	noteNodes[29]->name = 29;
	noteNodes[30] = &node30;
	noteNodes[30]->name = 30;
	noteNodes[31] = &node31;
	noteNodes[31]->name = 31;
	noteNodes[32] = &node32;
	noteNodes[32]->name = 32;
	noteNodes[33] = &node33;
	noteNodes[33]->name = 33;
	noteNodes[34] = &node34;
	noteNodes[34]->name = 34;
	noteNodes[35] = &node35;
	noteNodes[35]->name = 35;
	noteNodes[36] = &node36;
	noteNodes[36]->name = 36;
	noteNodes[37] = &node37;
	noteNodes[37]->name = 37;
	noteNodes[38] = &node38;
	noteNodes[38]->name = 38;
	noteNodes[39] = &node39;
	noteNodes[39]->name = 39;
	noteNodes[40] = &node40;
	noteNodes[40]->name = 40;
	noteNodes[41] = &node41;
	noteNodes[41]->name = 41;
	noteNodes[42] = &node42;
	noteNodes[42]->name = 42;
	noteNodes[43] = &node43;
	noteNodes[43]->name = 43;
	noteNodes[44] = &node44;
	noteNodes[44]->name = 44;
	noteNodes[45] = &node45;
	noteNodes[45]->name = 45;
	noteNodes[46] = &node46;
	noteNodes[46]->name = 46;
	noteNodes[47] = &node47;
	noteNodes[47]->name = 47;
	noteNodes[48] = &node48;
	noteNodes[48]->name = 48;
	noteNodes[49] = &node49;
	noteNodes[49]->name = 49;
	noteNodes[50] = &node50;
	noteNodes[50]->name = 50;
	noteNodes[51] = &node51;
	noteNodes[51]->name = 51;
	noteNodes[52] = &node52;
	noteNodes[52]->name = 52;
	noteNodes[53] = &node53;
	noteNodes[53]->name = 53;
	noteNodes[54] = &node54;
	noteNodes[54]->name = 54;
	noteNodes[55] = &node55;
	noteNodes[55]->name = 55;
	noteNodes[56] = &node56;
	noteNodes[56]->name = 56;
	noteNodes[57] = &node57;
	noteNodes[57]->name = 57;
	noteNodes[58] = &node58;
	noteNodes[58]->name = 58;
	noteNodes[59] = &node59;
	noteNodes[59]->name = 59;
	noteNodes[60] = &node60;
	noteNodes[60]->name = 60;
	noteNodes[61] = &node61;
	noteNodes[61]->name = 61;
	noteNodes[62] = &node62;
	noteNodes[62]->name = 62;
	noteNodes[63] = &node63;
	noteNodes[63]->name = 63;
	noteNodes[64] = &node64;
	noteNodes[64]->name = 64;
	noteNodes[65] = &node65;
	noteNodes[65]->name = 65;
	noteNodes[66] = &node66;
	noteNodes[66]->name = 66;
	noteNodes[67] = &node67;
	noteNodes[67]->name = 67;
	noteNodes[68] = &node68;
	noteNodes[68]->name = 68;
	noteNodes[69] = &node69;
	noteNodes[69]->name = 69;
	noteNodes[70] = &node70;
	noteNodes[70]->name = 70;
	noteNodes[71] = &node71;
	noteNodes[71]->name = 71;
	noteNodes[72] = &node72;
	noteNodes[72]->name = 72;
	noteNodes[73] = &node73;
	noteNodes[73]->name = 73;
	noteNodes[74] = &node74;
	noteNodes[74]->name = 74;
	noteNodes[75] = &node75;
	noteNodes[75]->name = 75;
	noteNodes[76] = &node76;
	noteNodes[76]->name = 76;
	noteNodes[77] = &node77;
	noteNodes[77]->name = 77;
	noteNodes[78] = &node78;
	noteNodes[78]->name = 78;
	noteNodes[79] = &node79;
	noteNodes[79]->name = 79;
	noteNodes[80] = &node80;
	noteNodes[80]->name = 80;
	noteNodes[81] = &node81;
	noteNodes[81]->name = 81;
	noteNodes[82] = &node82;
	noteNodes[82]->name = 82;
	noteNodes[83] = &node83;
	noteNodes[83]->name = 83;
	/*Declare Dummy nodes to start with and fall back on when no notes
	 *are being played, These ensure silence*/
	noteNodes[84] = &dummy1;
	noteNodes[84]->name = 84;
	noteNodes[85] = &dummy2;
	noteNodes[85]->name = 84;
	noteNodes[86] = &dummy3;
	noteNodes[86]->name = 84;
	
	//Set dummy nodes for initial no notes
	head = noteNodes[84];
	head->previous = NULL;
	head->next = noteNodes[85];
	head->next->previous = head;
	head->next->next = noteNodes[86];
	head->next->next->previous = head->next;
}

/*When a note on message is received that note's node is added to the start of
 *the noteBuffer linked list and made the head*/
void note_on(uint8_t noteIndex)
{
	//Cross Reference: check if this note is already being played
	//note off must have been lost in speed of FTDI header buffer
	if (notesOn[noteIndex] == 0xFF) {
		//Note was already on, turn it off
		note_off(noteIndex);
	}
	tempHead = noteNodes[noteIndex];
	tempHead->previous = NULL;
	tempHead->next = head;
	head->previous = tempHead;
	head = tempHead;
	//Set the note on array
	notesOn[noteIndex] = 0xFF;
}

/*When a note off message is received that note's node is removed from the 
 *noteBuffer linked list. If it was the head, head is updated*/
void note_off(uint8_t noteIndex)
{
	volatile struct noteNode* current = head;
	//search list for the note to turn off
	while (current != NULL) {
		if (current->name == noteIndex) {
			//This is the note to turn off
			if (current->previous == NULL) { //It is the Head
				head = current->next; //Next is the new head
				head->previous = NULL; //Set previous to null for new head
			} else { //The note was not the head
				current->previous->next = current->next;
				current->next->previous = current->previous;
			}
			break;
		}
		current = current->next;
	}
	//Set note on array
	notesOn[noteIndex] = 0x00;
}

/*Sets up the clock for NCO's 60.5kHz*/
void init_ref_clock(void)
{
	TCNT0 = 0;
	//Set timer in CTC mode
	//Compare to OCR0A
	TCCR0A |= (1 << WGM01);
	//Out, compare match for 60.5KHz, is 40
	//given divided interrupt: Refclk is 31.25KHz
	OCR0A = 40;
	//Prescaler of 8
	TCCR0B |= (1 << CS01);
	//Turn on timer compare interrupts
	TIMSK0 |= (1<<OCIE0A);
	//Ensure Interrupts are clear at the start
	TIFR0 &= (1<<OCF0A);
}

/*Checks the 4 pole switch on PIND and sets the waveform accordingly*/
void check_switch(void)
{
	switchState = PIND & 0b00111100;
	if (switchState != prevSwitchState) {
		switch (switchState) { //exclude all but pins switch is connected to
			case 0b00000100: //Switch activating D2: Saw Wave
			waveForm = &sawLut[0];
			break;
			case 0b00001000: //Switch activating D3: Sine wave
			waveForm = &sinLut[0];
			break;
			case 0b00010000: //switch activating D4: square wave
			waveForm = &squareLut[0];
			break;
			case 0b00100000: //switch activating D5: triangle wave
			waveForm = &triLut[0];
			break;
			default:
				waveForm = &sinLut[0];
		}
		prevSwitchState = switchState;
	}
}

/*Interrupt Handler for a pin change on PINS D2 - D5*/
ISR(PCINT3_vect)
{
	_delay_ms(100);
	switch (PIND & 0b00111100) { //exclude all but pins switch is connected to
		case 0b00000100: //Switch activating D2: Saw Wave
			waveForm = &sawLut[0];
			break;
		case 0b00001000: //Switch activating D3: Sine wave
			waveForm = &sinLut[0];
			break;
		case 0b00010000: //switch activating D4: square wave
			waveForm = &squareLut[0];
			break;
		case 0b00100000: //switch activating D5: tri wave
			waveForm = &triLut[0];
			break;
	}
}

/*Reads the UART buffer and completes operation set by the 
 *Control Byte*/
void read_uart(void)
{
	//Reset Count back to 0;
	uartCount = 0;
	//Byte1 is config
	if (uartBuffer[0] == 0xF0) {
		UDR0 = uartBuffer[1];
		note_on(uartBuffer[1]);
	} else if (uartBuffer [0] == 0x80) {
		note_off(uartBuffer[1]);
	} else if ((uartBuffer[0] & 0xF0) == 0b10100000) {
		//Button Setting message
		set_save_button((uartBuffer[0] & 0x0F), uartBuffer[1]);
	}
	//Update the playing notes linked list
	tword1 = tune[head->name];
	tword2 = tune[head->next->name];
	tword3 = tune[head->next->next->name];
}

/*Checks buttons 0 to 3 on the lower nibble of PINA*/
void check_low_buttons(void)
{
	//Check buttons 1 - 4 lower Nibble of PINA
	buttonState1 = (PINA & 0x0F);
	for (uint8_t pin = 0; pin <= 3; ++pin) {
		if ((buttonState1 & (1 << pin)) && !(prevButtonState1 & (1 << pin))) {
			//Button Now On
			note_on(get_lower_button_tword((1 << pin)));
			tword1 = tune[head->name];
			tword2 = tune[head->next->name];
			tword3 = tune[head->next->next->name];
			break;
		}
		if (!(buttonState1 & (1 << pin)) && (prevButtonState1 & (1 << pin))) {
			//Button Now Off
			note_off(get_lower_button_tword((1 << pin)));
			tword1 = tune[head->name];
			tword2 = tune[head->next->name];
			tword3 = tune[head->next->next->name];
			break;
		}
	}
	prevButtonState1 = buttonState1;
}

/*Sets the data direction register for the sound playing LED on pin A5*/
void init_sound_led(void)
{
	DDRA |= (1 << DDRA5);
}

/*Turns the sound playing indicator LED on*/
void sound_led_on(void)
{
	PORTA |= (1 << PORTA5);
}

/*Turns the sound playing indicator LED off*/
void sound_led_off(void)
{
	PORTA &= ~(1 << PORTA5);
}

/*Checks buttons 4 to 7 on the lower nibble of PINB*/
void check_high_buttons(void)
{
	//Check buttons 5 - 8 on lower Nibble of PINB
	buttonState2 = (PINB & 0x0F);
	for (uint8_t pin2 = 0; pin2 <= 3; ++pin2) {
		if ((buttonState2 & (1 << pin2)) && !(prevButtonState2 & (1 << pin2))) {
			//Button Now On
			note_on(get_upper_button_tword((1<<pin2)));
			tword1 = tune[head->name];
			tword2 = tune[head->next->name];
			tword3 = tune[head->next->next->name];
			break;
		}
		if (!(buttonState2 & (1 << pin2)) && (prevButtonState2 & (1 << pin2))) {
			//Button Now Off
			note_off(get_upper_button_tword((1<<pin2)));
			tword1 = tune[head->name];
			tword2 = tune[head->next->name];
			tword3 = tune[head->next->next->name];
			break;
		}
	}
	prevButtonState2 = buttonState2;
}

/*sets up loop for phase accumulator interrupt to occur in.
 *Everything Goes on inside this loop*/
void play_sounds(void)
{
	populate_note_buffer();
	init_sound_led();
	//Set initial waveform
	//waveForm = &sinLut[0];
	//waveForm = &sawLut[0];
	//waveForm = &triLut[0];
	//waveForm = &squareLut[0];
	
	while(1) {
		get_POT_set_TWIPOT();
		check_low_buttons();
		check_high_buttons();
		check_switch();
		
		//2 Byte buffer full with a message, read it
		if (uartCount > 1) {
			read_uart();
		}
		//Ensure notes off are off
		if (tword1 == 0) {
			sound_led_off();
			phacc1 = 0;
		}
		if (tword2 == 0) {
			phacc2 = 0;
		}
		if (tword3 == 0) {
			phacc3 = 0;
		}
		//NCO 1 will be always the first and last used
		//Use to trigger the Sound playing LED, and disabling the timeout
		if (tword1 != 0) {
			reset_timeout();
			sound_led_on();
		}
	}
}

/*Interrupt for the REF_CLK at 31.25KHz.*/
ISR(TIMER0_COMPA_vect)
{	
	if(check) {
	
	spi_send(waveForm[(phacc1 >> 8)], 1);
	
	spi_send(waveForm[(phacc2 >> 8)], 2);
	
	spi_send(waveForm[(phacc3 >> 8)], 3);
	check--;
	} else {
		phacc1 += tword1;
		if ((phacc1 >> 8) > SAMPLE_NUM) {
			phacc1 -= 0xF0;
		}
		phacc2 += tword2;
		if ((phacc2 >> 8) > SAMPLE_NUM) {
			phacc2 -= 0xF0;
		}
		phacc3 += tword3;
		if ((phacc3 >> 8) > SAMPLE_NUM) {
			phacc3 -= 0xF0;
		}
		check++;
	}
}

/*Receive complete interrupt handler for the USART.
 *Puts the byte into a buffer*/
ISR(USART0_RX_vect)
{
	//Flash RX LED on PORTC7
	enable_RX_LED();

	uartBuffer[uartCount] = UDR0;
	//0xFF Resets the buffer
	if (uartBuffer[uartCount] == 0xFF) {
		uartCount = 0;
		PORTC &= ~(1 <<PORTC7);
		return;
	}
	uartCount++;
	
	//put LED back low
	disable_RX_LED();
}