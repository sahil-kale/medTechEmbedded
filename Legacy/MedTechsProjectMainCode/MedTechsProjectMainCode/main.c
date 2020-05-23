/*
* Copyright Sahil Kale 2020
* Github: @dakaleyt | sahil.kale1@gmail.com
* All rights reserved
*
* Purpose:
* Revision: 1.13
* Microcontroller: ATMega328p
 */ 

/*
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>	
#include <stdlib.h>

#define baudRate9600 5 //9600 BPS baud rate. The code is 71.

//Baud rate is calculated by Fosc / (16 * desired baud rate) - 1

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit)) //uses an XOR gate. Basically, if both bits are the same, 0 is returned. By using a togglebit macros, we can target a certain bit by entering ex: 10101010 with 00000100 in order to toggle the bit.


void blinkLED() //blinks the led. Ports are hardcoded.
{
	toggleBit(PORTB, PB0);
	_delay_ms(500);
	toggleBit(PORTB, PB0);
	_delay_ms(500);
}

void USART_TransmitChar(unsigned char data)
{
	//Blink when its transmitting
	blinkLED();
		
	//while(isBitSet(PINB, PB6)); //ensures that the program stalls itself if CTS is false
	
		while (isBitClear(UCSR0A, UDRE0)) //If UDRE0 0 bit is set to 1, the transmitter is ready to transmit again.
		{ 
			//This piece of code stalls the program when transmitting. RTS is set to high to indicate UNSAFE
			//if(!isBitSet(PORTB, PB7))
			//{
			//	toggleBit(PORTB, PB7);
			//}
			
		}
		//if(!isBitClear(PORTB, PB7))
		//{
			//toggleBit(PORTB, PB7);
		//}
		//Set RTS to low to indicate SAFE
		UDR0 = data;
}

void UART_putString(char* stringA)
{
	while(*stringA != 0x00)
	{
		USART_TransmitChar(*stringA);
		stringA++;
	}
	
}



int ADCsingleRead(uint8_t adcPort) //adcPort argument takes an integer from 0-8 that will specify the ADC to use. Easier than hard coding the port so that in the future, we can call the function :)
{
	int returnValue;
	ADMUX = 0b00000000;
	ADMUX = adcPort;
	ADMUX |= (1 << REFS0) | (1 << ADLAR); //AVcc internal reference, left justify ADLAR. Plan is to output ADCH for now since this is a test
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC);
	
	while(isBitSet(ADCSRA, ADSC))
	{
		
	}
	
	returnValue = 1;
	return returnValue;
	//returnValue = (ADCH << 8) + ADCL;	//Use this if ADLAR is right justified to get 10 bit resolution.

}

void init()
{
	DDRB &= (0x00); //Clear DDRB, and also sets PB6 as an input as it is the CTS.
	PORTB &= (0x00); //Clear PORTB register. This also sets the RTS pin, PB7, to low.
	//PORTB |= (1 << PB6); //Use internal pull-up resistor in order to use PinX for determining output. 0 = logic low.
	//DDRB |= (1 << PB7); // Set the RTS, PB7 as an output. We can now use PortB bit 7 to set high or low.
	DDRB |= (1 << PB0); //Set the LED
	
	//turn the led on.
	if(isBitSet(PORTB, PB0))
	{
		toggleBit(PORTB, PB0);
	}
	
	unsigned int ubrr = baudRate9600; //universal baud rate generator
	
	//ADC Code:
	ADMUX = 0b00000000;
	ADCSRA = 0b00000000;
	ADCSRB = 0b00000000;
	
	//UART Code:
	UCSR0C = 0x06;  //This is to set the frame format with 1 stop bit and no parity bits
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);  //Starts the cannon...I mean transmitter
	UBRR0H = (ubrr>>8);
	UBRR0L = (ubrr);	
}


int main(void)
{
	init();
	
    while (1) 
    {
		
		int tempReading = ADCsingleRead(5); //single read the ADC off of ADC 0
		char tempBuffer[11];
		itoa(tempReading, tempBuffer, 2);
		char tempString[] = "Temperature: ";
		UART_putString(tempString);
		UART_putString(tempBuffer);
		
		USART_TransmitChar(ADCH); //Will only be 2 bits.
		USART_TransmitChar(ADCL); //ADCL and ADCH is where we are storing the ADC output. it is then put in the USART Transmit function which asks for a char to transmit the raw data from the high register. 		
    }
}
*/


//ADC Test Code: This code works!!
#ifndef F_CPU
#define F_CPU 1000000UL
#endif



#include <avr/io.h>
#include <util/delay.h>

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))

void blinkLED() //blinks the led. Ports are hardcoded.
{
	toggleBit(PORTB, PB0);
	_delay_ms(500);
	toggleBit(PORTB, PB0);
	_delay_ms(500);
}

int ADCsingleRead(uint8_t adcPort) //adcPort argument takes an integer from 0-8 that will specify the ADC to use. Easier than hard coding the port so that in the future, we can call the function :)
{
	int returnValue;
	ADMUX = 0b00000000;
	ADMUX = adcPort;
	ADMUX |= (1 << REFS0) | (1 << ADLAR); //AVcc internal reference, left justify ADLAR. Plan is to output ADCH for now since this is a test
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC);
	
	while(isBitSet(ADCSRA, ADSC))
	{
		
	}
	
	returnValue = 1;
	return returnValue;
	//returnValue = (ADCH << 8) + ADCL;	//Use this if ADLAR is right justified to get 10 bit resolution.
}
int main(void)
{
	DDRB = 0b11111111;
	DDRD |= 0b11111111;
	//DDRC |= 0b11111111;
	 //We're gonna use Port D for output :D
	//Note: the input pin is specified in the ADMUX register.
	
	//reset all registers.
	//ADMUX = 0b01100101;
	//ADCSRA = 0b10000011;
	ADCSRA = 0b00000000;
	ADCSRB = 0b00000000;
	
	PORTD = 0b11111111;
	PORTB = 0b11111111;
//	_delay_ms(500);
	
    while (1) 
    {
		int notUsed = ADCsingleRead(5);
		PORTB = ADCL; 
		PORTD = ADCH;
	
		
		//blinkLED();
		
		
    }
	return 0;
}

/*
#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1000000UL			// define it now as 1 GHz unsigned long
#endif

#include <avr/io.h>				// this is always included in AVR programs

#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
	
	DDRD = 0xFF;				// set Port D pins for output
	
  
	ADMUX = 0b01100101;
	
	ADCSRA = 0b10000011;

	ADCSRB = 0b00000000;
	
	while (1) {									// begin infinite loop
		ADCSRA |= (1 << ADSC);					// start ADC conversion
		while(BIT_IS_SET(ADCSRA, ADSC)) {}		// wait here until the chip clears the ADSC bit for us, which means the ADC is complete
		PORTD = ADCH;
		PORTC = ADCL;							// assign contents of ADC high register to Port D pins
	}
	return(0);					// should never get here, this is to prevent a compiler warning
}
*/
