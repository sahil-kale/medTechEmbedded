/*
* Copyright Sahil Kale 2020
* Github: @dakaleyt | sahil.kale1@gmail.com
* All rights reserved
*
* Purpose: MedTechs main code
* Revision: 2.0
* Microcontroller: ATMega328/p
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define baudRate 25 //Fosc/(16*baudRate), Baud rate of 2400

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))

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


void USART_putChar(unsigned char data)
{
	while (isBitClear(UCSR0A, UDRE0)) //If UDRE0 0 bit is set to 1, the transmitter is ready to transmit again.
	{}
	UDR0 = data;
}

void UART_putString(char* stringA)
{
	while(*stringA != 0x00)
	{
		USART_putChar(*stringA);
		stringA++;
	}
}

unsigned char UART_receiveChar()
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

void UART_init(uint16_t ubrr) //takes in baud rate number
{
	// set baudrate in UBRR
	UBRR0L = (uint8_t)(ubrr & 0xFF); //gets low bits for baudrate
	UBRR0H = (uint8_t)(ubrr >> 8); //gets high bits for baudrate

	UCSR0C = (1 << USBS0)|(3 << UCSZ00); //Set frame format: 8data, 2stop bit

	// enable the transmitter and receiver
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	_delay_ms(5000);
}

void blinkLED() //blinks the led. Ports are hardcoded.
{
	toggleBit(PORTB, PB0);
	_delay_ms(500);
	toggleBit(PORTB, PB0);
	_delay_ms(500);
}

void init()
{
	unsigned int ubrr = baudRate;
	UART_init(ubrr);
	DDRB |= 0b00000001;	
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
		char tempReceiveChar = 'T';
		if(UART_receiveChar() == tempReceiveChar)
		{
			char temperatureString[] = "TempC";
			transmitADCvalues(5, temperatureString);
			
			
			USART_putChar('\n'); //new line
		}
		
		char blinkReceiveChar = 'B';
		if(UART_receiveChar() == blinkReceiveChar)
		{
			blinkLED();
		}
		
    }
	return 0;
}

//Old Code

/*
		int tempReading  = ADCsingleRead(5); //ADC Converter gang
		char tempString[] = "TempC";
		char tempBuffer[11]; //Create a char buffer to write the reading too. Calculated by # of chars + 1 for null bit
		itoa(tempReading, tempBuffer, 2); //Converts the temp reading into a string that is stored in tempBuffer
		UART_putString(tempBuffer);
		UART_putString(tempString);
*/