/*
 * MainCodeMedTechEmbedded.c
 *
 * Created: 5/19/2020 23:10:13
 * Author : sahil
 */

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define baudRate10416 25 //Fosc/(16*baudRate)

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))

int ADCsingleRead(uint8_t adcPort) //adcPort argument takes an integer from 0-8 that will specify the ADC to use. Easier than hard coding the port so that in the future, we can call the function :)
{
	int returnValue;
	ADMUX = 0b00000000;
	ADCSRA = 0b00000000;
	ADCSRB = 0b00000000;
	
	ADMUX = adcPort;
	ADMUX |= (1 << REFS0) | (0 << ADLAR); //AVcc internal reference, left justify ADLAR. Plan is to output ADCH for now since this is a test
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC);
	
	while(isBitSet(ADCSRA, ADSC)) //stalls the code while the ADC is initalizing/running
	{}
		
	unsigned int fakeADCL = ADCL;
	unsigned int fakeADCH = ADCH;	
	returnValue = (fakeADCH << 8) + fakeADCL;
	return returnValue;
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
}


void UART_init(uint16_t ubrr) //takes in baud rate number
{
	// set baudrate in UBRR
	UBRR0L = (uint8_t)(ubrr & 0xFF); //gets low bits
	UBRR0H = (uint8_t)(ubrr >> 8); //gets high bits
	//UCSR0C = 0x06;  //Refer to data sheet for things 19.10.4


	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);

	// enable the transmitter and receiver
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
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
	unsigned int ubrr = baudRate10416;
	UART_init(ubrr);
	
	DDRB |= 0b00000001;	
	blinkLED();
	
}

int main(void)
{
	
	init();
	
    /* Replace with your application code */
    while (1) 
    {
		int tempReading  = ADCsingleRead(5);
		char tempString[] = "TempC";
		char tempBuffer[11];
		itoa(tempReading, tempBuffer, 2);
		UART_putString(tempBuffer);
		USART_TransmitChar('\n');
		UART_putString(tempString);	
    }
	return 0;
}

