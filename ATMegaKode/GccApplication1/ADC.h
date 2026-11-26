/*
 * ADC.h
 *
 * Created: 26/11/2026 11.20.11
 *  Author: Micha
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif /* ADC_H_ */