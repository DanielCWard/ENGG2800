/*
 * I2C.c
 *
 * Created: 21/10/2015 10:27:33 PM
 *  Author: Dan
 */ 

/*This I2C code is for the AD5241 Digital Potentiometer*/
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>

/*A function that waits for I2C transmission completion*/
void wait_for_transmission(void)
{
	while(!(TWCR & (1 << TWINT))) {
		;
	}
}

/*Sends the start condition and the address to the I2C device
adr1 and adr2 are the address bits: LOW or HIGH
RW is the read/write selection: LOW == write, HIGH == READ*/
void i2c_send_byte_1(uint8_t adr1, uint8_t adr2, uint8_t RW)
{
	static uint8_t devAddr = 0x58; //Manufacturer set address bits
	//Bit shift in the given device address
	devAddr |= (adr1 << 2); //address bit one in 3rd LSB
	devAddr |= (adr2 << 1); //address bit 2 in 2nd LSB
	//Set read or write mode, LSB
	devAddr |= (RW << 0);
	uint8_t status = 0x00;
	//Send start byte
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	wait_for_transmission();
	//Check TWI StatReg
	status = (TW_STATUS & 0xF8); //Mask prescalers
	if ((status != TW_START) && (status != TW_REP_START)) {
		return; //Failed
	}
	//Send Device Address
	TWDR = devAddr;
	TWCR = (1 << TWINT) | (1 << TWEN);
	wait_for_transmission();
	//Check TWI StatReg 
	status = (TW_STATUS & 0xF8); //Mask prescalers
	if ((status != TW_MT_SLA_ACK) && (status != TW_MR_SLA_ACK)) {
		return; //Failed
	}
	
}

/*sends the 2nd of the 3byte message to the device, the config byte.
out1: LOW == disable output 1, HIGH == enable output 1
out2: LOW == disable output 2, HIGH == enable output 2
rdac: Selects internal resistor: LOW == RDAC1, HIGH == RADC2*/
void i2c_send_byte_2(uint8_t out1, uint8_t out2, uint8_t rdac)
{
	uint8_t status = 0x00;
	uint8_t config = 0x00; 
	//Set outputs
	config |= (out1 << 4);
	config |= (out2 << 3);
	//Select RDAC
	config |= (rdac << 7);
	//Send config byte to previously addressed device
	TWDR = config;
	TWCR = (1 << TWINT) | (1 << TWEN);
	wait_for_transmission();
	//Check TWI StatReg
	status = (TW_STATUS & 0xF8); //Mask prescalers
	if (status != TW_MT_DATA_ACK) {
		return; //Failed
	}
}

/*Sends the 3rd byte to the device, the data byte
Data is the 8 bit integer to send to the device*/
void i2c_send_byte_3(uint8_t data)
{
	uint8_t status = 0x00;
	//Send data byte to previously addressed device
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	wait_for_transmission();
	//Check TWI StatReg
	status = (TW_STATUS & 0xF8); //Mask prescalers
	if (status != TW_MT_DATA_ACK) {
		return; //Failed
	}
}

/*Initializes the TWI/I2C interface
Assumes 20MHz FCPU*/
void init_i2c(void)
{
	//Prescaler of 1
	TWSR = 0x00;
	TWBR = 0x0C * 5;
	
	//TWI enable byte written in send byte functions
}

/*Closes the I2C connection bus to that device*/
void i2c_stop_byte(void)
{
	//Send stop byte
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	//Wait for execution and freed connection
	while(TWCR & (1 << TWSTO)) {
		;;
	}
}

/*Disables the i2c communication for Power saving mode*/
void disable_i2c(void)
{
	TWCR = 0x00;
}