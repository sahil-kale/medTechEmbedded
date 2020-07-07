/*
 * IncFile1.h
 *
 * Created: 6/12/2020 23:28:27
 *  Author: sahil
 */ 


#ifndef MAININCLUDE_H_
#define MAININCLUDE_H_

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>


#define baudRate 0 //Fosc/(16*baudRate), Baud rate of 31250

#define isBitSet(byte, bit) (byte & (1 << bit))
#define isBitClear(byte, bit) !(byte & (1 << bit))
#define toggleBit(byte, bit) (byte ^= (1 << bit))



#endif /* INCFILE1_H_ */