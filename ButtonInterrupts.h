/*
 * ButtonInterrupts.h
 *
 * Created: 22/10/2015 10:37:44 AM
 *  Author: Dan
 */ 


#ifndef BUTTONINTERRUPTS_H_
#define BUTTONINTERRUPTS_H_

/*Turns on pin toggle interrupts for the 8 piano keys as a wake up method*/
void init_button_interrupts(void);

/*Disables the Pin toggles to resume use of the piano Keys as piano Keys*/
void disable_button_interrupts(void);





#endif /* BUTTONINTERRUPTS_H_ */