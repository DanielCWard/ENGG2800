/*
 * USART.h
 *
 * Created: 13/09/2015 3:39:05 PM
 *  Author: Dan
 */ 


#ifndef USART_H_
#define USART_H_


/*Init USART, baud rate 2400*/
void init_serial(uint16_t baud);

/*Disables the USART for power saving mode*/
void disable_usart(void);

//Send an 8 bit number to USART
void send_num(uint8_t number);

/*Turns off the USART Receive LED on C7*/
void disable_RX_LED(void);

/*Turns on the usart receive LED on C7*/
void enable_RX_LED(void);

#endif /* USART_H_ */