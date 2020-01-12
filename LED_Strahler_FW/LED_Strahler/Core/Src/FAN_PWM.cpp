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
	uint16_t i;

	//Set duty cycle to zero
	FAN_PWM_SetDutyCycle(0);

	//Start FAN PWM
	HAL_TIM_PWM_Start(&FAN_PWM_TIMER, TIM_CHANNEL_1);

	//Ramp fan up and down for testing
	for(i = 0; i <= 1000; i++)
	{
		FAN_PWM_SetDutyCycle(i);
		HAL_Delay(1); //Delay 1ms
	}
	for(i = 1000; i <= 1000; i--)
	{
		FAN_PWM_SetDutyCycle(i);
		HAL_Delay(1); //Delay 1ms
	}
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
