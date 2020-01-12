/*
 * FAN_PWM.c
 *
 *  Created on: Nov 3, 2019
 *      Author: KaiLi
 */

#include "FAN_PWM.h"



extern TIM_HandleTypeDef htim17;



void FAN_PWM_Init(void)
{
	//Set duty cycle to 100% for testing
	FAN_PWM_SetDutyCycle(1000);

	//Start FAN PWM
	HAL_TIM_PWM_Start(&FAN_PWM_TIMER, TIM_CHANNEL_1);

	//Let fan ramp up shortly for testing
	HAL_Delay(4000);

	//Set duty cycle to zero
	FAN_PWM_SetDutyCycle(0);
}



void FAN_PWM_SetDutyCycle(uint16_t dc)
{
	if(dc > 1000)
	{
		dc = 1000;
	}

	dc = (uint16_t)((((uint32_t)dc) * 1920) / 1000);

	__HAL_TIM_SET_COMPARE(&FAN_PWM_TIMER, TIM_CHANNEL_1, dc);
}
