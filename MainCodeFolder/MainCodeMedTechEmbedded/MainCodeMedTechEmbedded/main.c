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
	if(firstBit) {UART_putString("1");}
		else {UART_putString("0");}
	bool secondBit = isBitSet(slaveSelectNumber, 1);
	if(secondBit) {UART_putString("1");}
		else {UART_putString("0");}
	bool thirdBit = isBitSet(slaveSelectNumber, 0);
	if(thirdBit) {UART_putString("1");}
		else {UART_putString("0");}
	
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
	
	//UART_putString("Function Executed");
	
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
				UART_putString("Blinking LED");
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
			case '3':
				slaveSelector(3);
				break;
			case '4':
				slaveSelector(4);
				break;
			case '5':
				slaveSelector(5);
				break;
			case '6':
				slaveSelector(6);
				break;
			case '7':
				slaveSelector(7);
			default:
				//UART_putString("Donkey");
				break;
		}
		
		UART_putChar('\n');
		
    }
	return 0;
}