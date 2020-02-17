/* 
 * File: pwm.h
 * Author: Miswired
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PWM_H
#define	PWM_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> //include C basic type definitions



void pwm_init(void);
uint8_t pwm_set_period_register(uint16_t period);
uint8_t pwm_enable_output(void);
uint8_t pwm_disable_output(void);
uint8_t pwm_set_period_tone(uint16_t period);
uint8_t pwm_set_duty_cycle_register(uint16_t dc);

#endif	/* XC_HEADER_TEMPLATE_H */

