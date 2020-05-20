/*
 * GccApplication1.c
 *
 * Created: 5/19/2020 19:30:58
 * Author : sahil
 */ 
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
#define toggleBit(byte, bit) (byte ^= (1 << bit))

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
	
	//while(isBitSet(PINB, PB6)); //ensures that the program stalls itself if CTS is false
	
	while (isBitClear(UCSR0A, UDRE0)) //If UDRE0 0 bit is set to 1, the transmitter is ready to transmit again.
	{
		
	}
	UDR0 = data;
	//blinkLED();
	
}

void UART_putString(char* stringA)
{
	while(*stringA != 0x00)
	{
		USART_TransmitChar(*stringA);
		stringA++;
	}
	
}

void UART_init(uint16_t ubrr)
{
	// set baudrate in UBRR
	UBRR0L = (uint8_t)(ubrr & 0xFF);
	UBRR0H = (uint8_t)(ubrr >> 8);
	UCSR0C = 0x06;

	// enable the transmitter and receiver
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}


int main(void)
{
	DDRB |= 0b00000001;
	blinkLED();
	int notUsed3 = 1;
	unsigned int ubrr = baudRate9600;
	UART_init(ubrr);
	
    /* Replace with your application code */
    while (1) 
    {
		char tempString[] = "Atharva likes POOP";
		UART_putString(tempString);
    }
}

