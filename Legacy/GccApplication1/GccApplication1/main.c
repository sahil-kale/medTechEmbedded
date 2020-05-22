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
	while (isBitClear(UCSR0A, UDRE0)) //If UDRE0 0 bit is set to 1, the transmitter is ready to transmit again.
	{}
	UDR0 = data;
}

void UART_putString(char* stringA)
{
	while(*stringA != 0x00)
	{
		USART_TransmitChar(*stringA);
		stringA++;
	}
	USART_TransmitChar('\n');

}

void UART_getChar(void)
{
	// wait for data
	while(isBitClear(UCSR0A, RXC0)))
	{
		
	}

	// return data
	return UDR0;
}

void UART_getLine(char* buf, uint8_t n)
{
	uint8_t bufIdx = 0;
	char c;

	// while received character is not carriage return
	// and end of buffer has not been reached
	do //The loop just increments itself
	{
		// receive character
		c = UART_getChar();

		// store character in buffer
		buf[bufIdx++] = c;
	}
	while((bufIdx < n) && (c != '\r'));

	// ensure buffer is null terminated
	buf[bufIdx] = 0;
}

void UARTping()
{
	uint8_t bufSize = 20;
	char buf[bufSize]; //create a temporary buffer.

	// get line from UART
	UART_getLine(buf, bufSize);

	// echo input
	UART_putString("You entered: ");
	UART_putString(buf);
	USART_TransmitChar('\n');
	
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

