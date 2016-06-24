/*
 * I2C.h
 *
 * Created: 21/10/2015 11:05:35 PM
 *  Author: Dan
 */ 


#ifndef I2C_H_
#define I2C_H_

#define HIGH 0x01
#define LOW 0x00

/*Sends the start condition and the address to the I2C device
adr1 and adr2 are the address bits: LOW or HIGH
RW is the read/write selection: LOW == write, HIGH == READ*/
void i2c_send_byte_1(uint8_t adr1, uint8_t adr2, uint8_t RW);


/*sends the 2nd of the 3byte message to the device, the config byte.
out1: LOW == disable output 1, HIGH == enable output 1
out2: LOW == disable output 2, HIGH == enable output 2
rdac: Selects internal resistor: LOW == RDAC1, HIGH == RADC2*/
void i2c_send_byte_2(uint8_t out1, uint8_t out2, uint8_t rdac);

/*Sends the 3rd byte to the device, the data byte
Data is the 8 bit integer to send to the device*/
void i2c_send_byte_3(uint8_t data);

/*Initializes the TWI/I2C interface
Assumes 20MHz FCPU*/
void init_i2c(void);

/*Closes the I2C connection bus to that device*/
void i2c_stop_byte(void);

/*Disables the i2c communication for Power saving mode*/
void disable_i2c(void);


#endif /* I2C_H_ */