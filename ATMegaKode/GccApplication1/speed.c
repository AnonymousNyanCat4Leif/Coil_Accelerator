/*
 * speed.c
 *
 * Created: 26/11/2026 11.21.12
 *  Author: Mads
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Speed.h"
#include <stdint.h>

volatile uint16_t overflow_count = 0;
volatile uint32_t elapsed_ticks = 0;
volatile uint8_t measurement_ready = 0;

#define SENSOR_distance 0.10 //Distance between CNY70

void speed_init(void){
	
	//TCCR1B |= (1 << CS11) | (1 << CS10); //Enable prescaler at 64-bit
	
	DDRD &= ~((1 << PD2) | (1 << PD3)); //Sets PD2 and PD3 as inputs from CNY70
	
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	
	MCUCR |= (1 << ISC01) | (1 << ISC00); //Configures INT0 for rising edge
	MCUCR |= (1 << ISC11) | (1 << ISC10); //Configures INT1 for rising edge
	
	GICR |= (1 << INT0); //GICR mask register, enable INT0. INT1 will be used later, the ball first has to past the first CNY70
	sei(); //Enable global interrupts
}

ISR(TIMER1_OVF_vect)
{
	overflow_count++; //Counts the number om Timer1 overflows
}

ISR(INT0_vect) //First CNY70 -> Starts measurement
{
	GICR &= ~(1 << INT0); //Disable INT0
	TCNT1 = 0; //Reset Timer1
	overflow_count = 0;
	
	TIFR = (1 << TOV1); //Clear Timer1 overflow flag
	TIMSK |= (1 << TOIE1); //Enable Timer1 overflow interrupt
	TCCR1B |= (1 << CS11) | (1 << CS10); //Start Timer1 with a prescaler 64
	
	GIFR = (1 << INTF1); //Clear INT1 flag
	GICR |= (1 << INT1); //Enable INT1
}

ISR(INT1_vect) //Second CNY70 -> Stops measurement
{
	GICR &= ~(1 << INT1); //Disable INT1
	TCCR1B = 0; //Stop timer 1
	TIMSK &= ~(1 << TOIE1); //Disable overflow interrupt
	
	elapsed_ticks = ((uint32_t)overflow_count * 65536UL) + TCNT1; //Calculate elapsed ticks and adds the timer
	
	GICR &= ~(1 << INT0); // Disable global measurement until reset
	
	measurement_ready = 1; //New measurement
}

uint8_t speed_is_ready(void)
{
	return measurement_ready; //Returns the new measurement
}

float speed_get_velocity(void)
{
	float time;
	
	time = elapsed_ticks * 4.0e-6; //The elapsed ticks are multiplied by the tick time of 4us
	
	return SENSOR_distance / time;
}

