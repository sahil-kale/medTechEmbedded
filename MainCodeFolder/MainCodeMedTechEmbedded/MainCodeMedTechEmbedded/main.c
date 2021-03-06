/*
* Copyright Sahil Kale 2020
* Github: @dakaleyt | sahil.kale1@gmail.com
* All rights reserved
*
* Purpose: MedTechs main code
* Revision: 2.0
* Microcontroller: ATMega328/p
 */ 



#include "mainInclude.h"
#include "UART.h"
#include "adc.h"

void slaveSelector(uint8_t slaveSelectNumber) //Number between 0-7
{
	
	
	DDRB |= 0b00000111; //Ensure that the pins are set to output
	
	//Acquire the individual bits for the slaveSelect byte
	bool firstBit = isBitSet(slaveSelectNumber, 0);
	bool secondBit = isBitSet(slaveSelectNumber, 1);
	//bool thirdBit = isBitSet(slaveSelectNumber, 2);
	
	//Acquire the current state of the bits in PORT B
	bool pin1State = isBitSet(PORTB, PB0);
	bool pin2State = isBitSet(PORTB, PB1);
	//bool pin3State = isBitSet(PORTB, PB2);
	
	//Debugging code for Dhananjay to ensure that the appropriate slave select is set
	/*
	if(thirdBit) {UART_putString("1");}
	else {UART_putString("0");}
			
	if(secondBit) {UART_putString("1");}
		else {UART_putString("0");}
			
	if(firstBit) {UART_putString("1");}
	else {UART_putString("0");}
	
	
	UART_putChar('\n');
	*/
	
	
	//Compare and set
	if(!(firstBit == pin1State))
	{
		toggleBit(PORTB, PB0);
	}
	
	if(!(secondBit == pin2State))
	{
		toggleBit(PORTB, PB1);
	}
	/*
	if(!(thirdBit == pin3State))
	{
		toggleBit(PORTB, PB2);
	}
	*/
	
}


void blinkLED() //blinks the led. Ports are hardcoded.
{
	DDRB |= 0b00000100;	
	toggleBit(PORTB, PB2);
	_delay_ms(500);
	toggleBit(PORTB, PB2);
	_delay_ms(500);
}

void init()
{
	unsigned int ubrr = baudRate;
	UART_init(ubrr);
	blinkLED();
	toggleBit(PORTB, PB2);
	
}

void transmitADCvalues(uint8_t ADCPort, char* stringToTransmit) //does the ADC conversion, then does the conversion and sends the raw ADC value and then the stringToTransmit.
{
	int reading = ADCsingleRead(ADCPort);
	UART_putChar(reading >> 8); //Write the 2 bits in the "high" register
	UART_putChar(reading); //write the remaining 8 bits
	
}

int main(void)
{
	
	init();	
	//toggleBit(PORTB, PB0);
	int counter = 0;
	int endCounter = 4095;
	
    /* Replace with your application code */
    while (1) 
    {
		
		char receivedChar = UART_getChar();
		switch(receivedChar) //massive switch case for commands
		{
			case 'B': //blink code
				UART_putString("Blinking LED");
				blinkLED();
				break;
			case '0':
				slaveSelector(0);
				transmitADCvalues(0, "");
				break;
			case '1':
				slaveSelector(1);
				while(counter < endCounter)
				{
					transmitADCvalues(0, "");
					counter++;
				}
				counter = 0;
				break;
			case '2':
				slaveSelector(2);
				while(counter < endCounter)
				{
					transmitADCvalues(0, "");
					counter++;
				}
				counter = 0;
				break;
			case '3':
				slaveSelector(3);
				while(counter < endCounter)
				{
					transmitADCvalues(0, "");
					counter++;
				}
				counter = 0;
				break;
			case 'T':
				slaveSelector(0);
				UART_putString("10");
				break;
			default:
				//UART_putString("Donkey");
				break;
		}
		
		
		UART_putChar('\n');
		
		
    }
	return 0;
}