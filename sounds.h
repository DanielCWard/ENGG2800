/*
 * sounds.h
 *
 * Created: 6/09/2015 8:50:24 AM
 *  Author: Daniel Ward
 */ 


#ifndef SOUNDS_H_
#define SOUNDS_H_



/*Sets up the clock for NCOs*/
void init_ref_clock(void);

/*sets up loop for phase accumulator interrupt to occur in*/
void play_sounds(void);

/*Plays the note in the global note message variables
 *Works out which NCO to output it on*/
void note_on(void);

/*Finds which NCO is outputting the note which is to be turned off
 *Sets that NCO to be next to used*/
void note_off(void);

/*Checks the 4 pole switch on PIND and sets the wavefrom accordingly*/
void check_switch(void);

#endif /* SOUNDS_H_ */