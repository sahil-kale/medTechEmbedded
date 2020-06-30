/*
 * PWMServoTest.c
 *
 * Created: 6/29/2020 17:18:23
 * Author : sahil
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))

//PB4 is the signal port

void pwm(uint16_t dutyCycle) //amount from 0-1
{
	int freq = 50;
	int period = 1/freq;
	
	int dutyCyclePeriod = period * dutyCycle;
	if(isBitClear(PORTB, PB4))
	{
		toggleBit(PORTB, PB4);
	}
	_delay_ms(dutyCyclePeriod);
	toggleBit(PORTB, PB4);
	_delay_ms(period-dutyCyclePeriod);
}


int main(void)
{
	int number = 0;
	toggleBit(PORTB, PB3);
    /* Replace with your application code */
    while (1) 
    {
		pwm(number + 0.001);
    }
}

