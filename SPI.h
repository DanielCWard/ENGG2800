/*
 * SPI.h
 *
 * Created: 1/09/2015 8:35:52 PM
 *  Author: Daniel Ward
 */ 


#ifndef SPI_H_
#define SPI_H_


/*Set up the SPI as a master to control the DAC(s)*/
void init_spi(uint8_t clockSpeed);

/*Sending byte over SPI, takes byte and destination
either DAC1, DAC2 or DAC3)*/
void spi_send(uint8_t byte, uint8_t dest);


/*Send out via parallel dac on PORT A*/
void para_dac_send(uint8_t byte, uint8_t dest);

/*Turns off the SPI communication for power saving mode*/
void disable_spi(void);

#endif /* SPI_H_ */