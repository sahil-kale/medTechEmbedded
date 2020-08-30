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


#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))

void blinkLED() //blinks the led. Ports are hardcoded.
{
	toggleBit(PORTC, PC3);
	_delay_ms(500);
	toggleBit(PORTC, PC3);
	_delay_ms(500);
}







int main(void)
{
	DDRC |= 0b11111111;
	blinkLED();

	
    /* Replace with your application code */
    while (1) 
    {
		blinkLED();
		
		
		
    }
}

