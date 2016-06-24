/*
 * PowerManagment.h
 *
 * Created: 9/10/2015 2:52:14 PM
 *  Author: Dan
 */ 


#ifndef POWERMANAGMENT_H_
#define POWERMANAGMENT_H_


/*Sets the data direction of PORTA5, and turns on the Power LED*/
void init_power(void);

/*Turn on Power LED*/
void enable_power_led(void);

/*Turn off Power LED*/
void disable_power_led(void);

/*Sets up a timer which if ever allowed to reach its compare value (5s)
 *will enforce power saving mode. If timeout occurs but device is connected
 *to the software package, no shutdown occurs*/
void init_timeout_timer(void);

/*Resets the timeout timer*/
void reset_timeout(void);

/*Turns off the Watch dog timer*/
void turn_off_wdt(void);

/*Turns on the Watchdog TImer*/
void turn_on_wdt(void);

/*Sets the watchdog timer timeout to 4 seconds*/
void wdt_set_timeout(void);

/*Restarts everything when the AVR has been woken up*/
void reload_all(void);

/*Shuts down different components of the firmware and puts the AVR into 
 *sleep mode, Can only be woken by pin change interrupts on any of the 
 *8 piano buttons (PINA0 - 3 and PINB0 - 3) Woken by the watchdog timer
 *every 5 seconds to flash the Power LED*/
void power_down(void);

/*Enables the peripherals, counteracts the power saving disable peripherals
 *Function*/
void enable_peripherals(void);

/*Sets the bits on the power reduction register to disable all peripherals for
 *Sleep mode*/
void disable_peripherals(void);

#endif /* POWERMANAGMENT_H_ */