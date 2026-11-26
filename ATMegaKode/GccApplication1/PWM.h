/*
 * PWM.h
 *
 * Created: 26/11/2026 11.19.44
 *  Author: Micha
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

void pwm_Init(void);
void pwm_set_speed(uint8_t duty);


#endif /* PWM_H_ */