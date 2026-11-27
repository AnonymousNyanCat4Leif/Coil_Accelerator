/*
 * speed.h
 *
 * Created: 26/11/2026 11.21.20
 *  Author: Mads
 */ 


#ifndef SPEED_H_
#define SPEED_H_

#include <stdint.h> //Libary for fixed sized intergers

void speed_init(void); //Initialiserer Timer1 and interrupts
uint8_t speed_isready(void); //New measure done
float speed_get_velocity(void); //Calculates speed
void speed_reset(void); //Gets ready for new measure

#endif /* SPEED_H_ */