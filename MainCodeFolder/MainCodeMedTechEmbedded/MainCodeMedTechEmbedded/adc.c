/*
 * CFile1.c
 *
 * Created: 6/12/2020 23:22:40
 *  Author: sahil
 */ 

#include "adc.h"

int ADCsingleRead(uint8_t adcPort) //adcPort argument takes an integer from 0-5 that will specify the ADC to use. Easier than hard coding the port so that in the future, we can call the function :)
{
	int returnValue;
	//Reset Registers
	ADMUX = 0b00000000;
	ADCSRA = 0b00000000;
	ADCSRB = 0b00000000;
	
	ADMUX = adcPort;
	ADMUX |= (1 << REFS0) | (0 << ADLAR); //AVcc internal reference, left justify ADLAR. Plan is to output ADCH for now since this is a test
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0); //Enable the ADC, divide input clock by 8 for the ADC.
	ADCSRA |= (1 << ADSC); //Fire the cannon... I mean ADC
	
	while(isBitSet(ADCSRA, ADSC)) //stalls the code while the ADC is initalizing/running
	{}
	
	//the fake values were put in to avoid issues with manipulating ADCH and ADCL
	unsigned int fakeADCL = ADCL;
	unsigned int fakeADCH = ADCH;
	returnValue = (fakeADCH << 8) + fakeADCL;
	return returnValue;
}