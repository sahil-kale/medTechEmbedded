/*
 * UART.h
 *
 * Created: 6/12/2020 23:13:40
 *  Author: sahil k
 */ 

#include "mainInclude.h"

#ifndef UART_H_
#define UART_H_

void UART_init(uint16_t ubrr);
void UART_putChar(unsigned char data);
void UART_putString(char* stringA);
unsigned char UART_getChar();

#endif
