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
	DDRB |= 0b00001110;
	bool firstBit = isBitSet(slaveSelectNumber, 2);
	bool secondBit = isBitSet(slaveSelectNumber, 1);
	bool thirdBit = isBitSet(slaveSelectNumber, 0);
	
	if(!(firstBit == isBitSet(PORTB, PB1)))
	{
		toggleBit(PORTB, PB1);
	}
	
	if(!(secondBit == isBitSet(PORTB, PB2)))
	{
		toggleBit(PORTB, PB2);
	}
	
	if(!(thirdBit == isBitSet(PORTB, PB3)))
	{
		toggleBit(PORTB, PB3);
	}
}


void blinkLED() //blinks the led. Ports are hardcoded.
{
	DDRB |= 0b00000001;	
	toggleBit(PORTB, PB0);
	_delay_ms(500);
	toggleBit(PORTB, PB0);
	_delay_ms(500);
}

void init()
{
	unsigned int ubrr = baudRate;
	UART_init(ubrr);
	blinkLED();
	
}

void transmitADCvalues(uint8_t ADCPort, char* stringToTransmit) //does the ADC conversion, then does the conversion and sends the raw ADC value and then the stringToTransmit.
{
	int reading = ADCsingleRead(ADCPort);
	char buffer[11];
	itoa(reading, buffer, 2);
	UART_putString(buffer);
	UART_putString(stringToTransmit);
	
}

int main(void)
{
	
	init();	
	
    /* Replace with your application code */
    while (1) 
    {
		char receivedChar = UART_getChar();
		switch(receivedChar)
		{
			case 'B': //blink code
				blinkLED();
				break;
			case 'T': //Temperature code				
				transmitADCvalues(5,"TempC");
				break;
			case '0':
				slaveSelector(0);
				break;
			case '1':
				slaveSelector(1);
				break;
			case '2':
				slaveSelector(2);
				break;
			default:
				UART_putString("Donkey");
				break;
		}
		
		UART_putChar('\n');
		
    }
	return 0;
}