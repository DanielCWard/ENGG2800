/*
 * _4PoleSwitch.h
 *
 * Created: 9/10/2015 4:32:55 PM
 *  Author: Dan
 */ 


#ifndef POLESWITCH_H_
#define POLESWITCH_H_


/*Sets up the Data direction and interrupt PCIE3 for the 4 pole switch 
connected to:
<Pin>: <WaveType> (<Interrupt>)
PIND2: SAW Wave (PCINT26)
PIND3: Sine Wave (PCINT27)
PIND4: Square Wave (PCINT28)
PIND5: Triangle Wave (PCINT29)
*/
void init_switch(void);

/*Disables the pin change interrupt for the 4 pole switch to ensure it 
 *doesn't wake up
 *the AVR from the Sleep mode*/
void disable_switch(void);


#endif /* 4POLESWITCH_H_ */