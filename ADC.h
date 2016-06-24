/*
 * ADC.h
 *
 * Created: 29/09/2015 3:48:54 PM
 *  Author: Dan
 */ 


#ifndef ADC_H_
#define ADC_H_

/*Reads the manual pot on synth board and sends the value to control
*the I2C POT on VCF.
*The digital pot should be a AD5241, this function puts the set resistance
*out on PORTB
*of the POT and uses RDAC 1
*Writes to resistance between pin 1 and wiper (pin 2)
*range 0 - 255, 0 is max resistance*/
void get_POT_set_TWIPOT(void);

/*Sets up the ADC*/
void init_adc(void);

/*starts the ADC conversion of the POT connected to PINA7*/
void start_ADC_conversion(void);

/*Gets the result of the last ADC conversion if ready*/
uint8_t get_ADC_result(void);

/*Sends the given 8bit value to the digital pot connected to i2c pins
 *The digital pot should be a AD5241, this function puts the set 
 *resistance out on PORTB of the POT and uses RDAc 1
 *Writes to resistance between pin 1 and wiper (pin 2)
 *range 0 - 255, 0 is max resistance*/
void set_POT(uint8_t val);

/*Turns off the ADC for power saving mode*/
void disable_adc(void);

#endif /* ADC_H_ */