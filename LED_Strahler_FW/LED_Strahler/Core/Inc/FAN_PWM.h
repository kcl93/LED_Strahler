/*
 * FAN_PWM.h
 *
 *  Created on: Nov 3, 2019
 *      Author: KaiLi
 */

#ifndef INC_FAN_PWM_H_
#define INC_FAN_PWM_H_

#include "stm32f0xx_hal.h"
#include "main.h"


#define FAN_PWM_TIMER		htim17


void FAN_PWM_Init(void);
void FAN_PWM_SetDutyCycle(uint16_t dc);


#endif /* INC_FAN_PWM_H_ */
