/*
 * LED_Thermomodel.c
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#include "LED_Thermomodel.h"
#include "stm32f0xx_hal.h"
#include "NTC.h"
#include "FAN_PWM.h"



float LED_Temp = 0.0;
float NTC_Temp = 0.0;
uint32_t Thermomodel_Timebase = 0;
uint16_t Fan_PWM = 0;


void LED_Thermomodel_Init(void)
{
	//Init FAN PWM
	FAN_PWM_Init();
	Fan_PWM = 0;

	//Initialize ADC
	NTC_Init();

	//Get initial temperature values
	NTC_Temp = NTC_Get_Temp();
	LED_Temp = NTC_Temp;

	//Setup timebase
	Thermomodel_Timebase = HAL_GetTick();
}



void LED_Thermomodel_Handle(void)
{
	if((HAL_GetTick() - Thermomodel_Timebase) > THERMOMODEL_PERIOD)
	{
		//Update and filter NTC temperature
		NTC_Temp = NTC_Temp + (0.125 * (NTC_Get_Temp() - NTC_Temp));

		//Calculate LED temperature based on thermal model (currently not implemented)
		LED_Temp = NTC_Temp;

		//Control FAN speed
		if(((Fan_PWM == 0) && (LED_Temp > THERMOMODEL_FAN_MIN_ON_TEMP)) || ((Fan_PWM != 0) && (LED_Temp > THERMOMODEL_FAN_MIN_TEMP)))
		{
			if(LED_Temp > THERMOMODEL_FAN_MAX_TEMP)
			{
				Fan_PWM = 1000;
			}
			else
			{
				Fan_PWM = (uint16_t)(((LED_Temp - THERMOMODEL_FAN_MIN_TEMP)*(float)(FAN_MAX_PWM - FAN_MIN_PWM)) / (THERMOMODEL_FAN_MAX_TEMP - THERMOMODEL_FAN_MIN_TEMP)) + FAN_MIN_PWM;
			}
		}
		else
		{
			Fan_PWM = 0;
		}
		FAN_PWM_SetDutyCycle(Fan_PWM);
	}
}



float LED_Thermomodel_GetTemp(void)
{
	return LED_Temp;
}


