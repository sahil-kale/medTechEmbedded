/*
 * CFile1.c
 *
 * Created: 6/12/2020 23:10:11
 *  Author: sahil
 */ 

#include "UART.h"

void UART_init(uint16_t ubrr) //takes in baud rate number
{
	//int ubrr = round(F_CPU/(16*baudRateTemp)-1);
	// set baudrate in UBRR
	UBRR0L = (uint8_t)(ubrr & 0xFF); //gets low bits for baudrate
	UBRR0H = (uint8_t)(ubrr >> 8); //gets high bits for baudrate

	UCSR0C = (1 << USBS0)|(3 << UCSZ00); //Set frame format: 8data, 2stop bit

	// enable the transmitter and receiver
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	_delay_ms(3000);
	
	UART_putString("UART Initialized");
	UART_putChar('\n');
	UART_putString("Copyright Sahil Kale 2020");
	UART_putChar('\n');
}

void UART_putChar(unsigned char data)
{
	while (isBitClear(UCSR0A, UDRE0)) //If UDRE0 0 bit is set to 1, the transmitter is ready to transmit again.
	{}
	UDR0 = data;
}

void UART_putString(char* stringA)
{
	while(*stringA != 0x00)
	{
		UART_putChar(*stringA);
		stringA++;
	}
}

unsigned char UART_getChar()
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)))
	{
		//blinkLED();
	}
	/* Get and return received data from buffer */
	return UDR0;
}