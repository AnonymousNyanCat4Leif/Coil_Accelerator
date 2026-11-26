(/*
 * IncFile1.h
 *
 * Created: 26/11/2026 11.02.15
 *  Author: Micha
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/interrupt.h>
#include <avr/io.h>

void USART_Init(unsigned int ubrr_value);
void USART_Print(const char *str);
int USART_Receive(void);

#endif /* USART_H_ */