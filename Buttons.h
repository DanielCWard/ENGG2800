/*
 * Buttons.h
 *
 * Created: 27/09/2015 9:37:28 AM
 *  Author: Dan
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

/*Initializes the buttons: Data direction and loads config from memory*/
void init_buttons(void);

//Saves button 0
void save_button_0(void);

//Saves button 1
void save_button_1(void);

//Saves button 2
void save_button_2(void);

//Saves button 3
void save_button_3(void);

//Saves button 4
void save_button_4(void);

//Saves button 5
void save_button_5(void);

//Saves button 6
void save_button_6(void);

//Saves button 7
void save_button_7(void);

/*Saves the Buttons array to EEPROM Memory*/
void save_buttons(void);

/*Sets the given button to the value and then saves it to EEPROM*/
void set_save_button(uint8_t buttonNum, uint8_t buttonVal);

/*Returns the 8bit value assigned to the given button, for buttons 0 - 3*/
uint8_t get_lower_button_tword(uint8_t buttonNum);

/*Returns the 8bit value assigned to the given button, for buttons 4 - 7*/
uint8_t get_upper_button_tword(uint8_t buttonNum);




#endif /* BUTTONS_H_ */