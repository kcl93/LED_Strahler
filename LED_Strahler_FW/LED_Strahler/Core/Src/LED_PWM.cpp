/*
 * LED_PWM.c
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#include "LED_PWM.h"


#define LED_R_CH_SLOW	TIM_CHANNEL_3
#define LED_R_CH_FAST	TIM_CHANNEL_1
#define LED_G_CH_SLOW	TIM_CHANNEL_4
#define LED_G_CH_FAST	TIM_CHANNEL_2
#define LED_B_CH_SLOW	TIM_CHANNEL_1
#define LED_B_CH_FAST	TIM_CHANNEL_3
#define LED_W_CH_SLOW	TIM_CHANNEL_2
#define LED_W_CH_FAST	TIM_CHANNEL_4



extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;



void LED_PWM_SetFastPWM(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_PWM_SetSlowPWM(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_PWM_SetPWM_TIM(uint32_t channel_slow, uint32_t channel_fast, uint16_t value);
uint16_t MapRounded(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);



uint8_t PWMboost = 0;
uint32_t PWMboostTime = 0;
uint16_t _led_red, _led_green, _led_blue, _led_white = 0;



void LED_PWM_Init(void)
{
	//Disable PWM boost
	PWMboost = 0;

	//Set duty cycles to 0
	LED_PWM_SetRGBW(0, 0, 0, 0);

	//Enable PWM output
	HAL_TIM_PWM_Start(&htim1, LED_R_CH_SLOW);
	HAL_TIM_PWM_Start(&htim1, LED_G_CH_SLOW);
	HAL_TIM_PWM_Start(&htim1, LED_B_CH_SLOW);
	HAL_TIM_PWM_Start(&htim1, LED_W_CH_SLOW);
	HAL_TIM_PWM_Start(&htim3, LED_R_CH_FAST);
	HAL_TIM_PWM_Start(&htim3, LED_G_CH_FAST);
	HAL_TIM_PWM_Start(&htim3, LED_B_CH_FAST);
	HAL_TIM_PWM_Start(&htim3, LED_W_CH_FAST);
}



void LED_PWM_SetRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	_led_red = red;
	_led_green = green;
	_led_blue = blue;
	_led_white = white;
	LED_PWM_SetPWM_TIM(LED_R_CH_SLOW, LED_R_CH_FAST, red);
	LED_PWM_SetPWM_TIM(LED_G_CH_SLOW, LED_G_CH_FAST, green);
	LED_PWM_SetPWM_TIM(LED_B_CH_SLOW, LED_B_CH_FAST, blue);
	LED_PWM_SetPWM_TIM(LED_W_CH_SLOW, LED_W_CH_FAST, white);
}



void LED_PWM_SetPWM_TIM(uint32_t channel_slow, uint32_t channel_fast, uint16_t value)
{
	uint16_t val_Slow = 65535; //Translates to 100%
	uint16_t val_Fast = 0; //Translates to 0%

	if(PWMboost == 0) // Limit max. output current while not boosting
	{
		value = (value * LED_PWM_STD_PEAK_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT;
	}

	if(value >= ((65535 * LED_PWM_ANALOG_MIN_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT))
	{ //Only analog dimming
		val_Fast = MapRounded(value, 0, 65535, (1000*LED_PWM_ANALOG_VOLT_MIN)/3300, (1000*LED_PWM_ANALOG_VOLT_MAX)/3300);
	}
	else
	{ //Mixed analog and PWM dimming
		val_Fast = MapRounded(((65535 * LED_PWM_ANALOG_MIN_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT), 0, 65535, (1000*LED_PWM_ANALOG_VOLT_MIN)/3300, (1000*LED_PWM_ANALOG_VOLT_MAX)/3300);
		val_Slow = MapRounded(value, 0, ((65535 * LED_PWM_ANALOG_MIN_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT), 0, 65535);
	}

	__HAL_TIM_SET_COMPARE(&LED_PWM_FAST_TIMER, channel_fast, val_Fast);
	__HAL_TIM_SET_COMPARE(&LED_PWM_SLOW_TIMER, channel_slow, val_Slow);
}



void LED_PWM_SetBoost(uint8_t active)
{
	PWMboost = active;
	PWMboostTime = HAL_GetTick();
}



void LED_PWM_CheckBoost(void)
{
	if((PWMboost != 0) && ((HAL_GetTick() - PWMboostTime) > LED_PWM_BOOST_TIMEOUT))
	{
		//Disable boost and DIM LEDs
		PWMboost = 0;
		_led_red = (_led_red * LED_PWM_STD_PEAK_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT;
		_led_green = (_led_green * LED_PWM_STD_PEAK_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT;
		_led_blue = (_led_blue * LED_PWM_STD_PEAK_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT;
		_led_white = (_led_white * LED_PWM_STD_PEAK_CURRENT) / LED_PWM_BOOST_PEAK_CURRENT;
		LED_PWM_SetRGBW(_led_red, _led_green, _led_blue, _led_white);
	}
}



void LED_PWM_SetFastPWM(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	if(red > 1000)
	{
		red = 1000;
	}
	if(green > 1000)
	{
		green = 1000;
	}
	if(blue > 1000)
	{
		blue = 1000;
	}
	if(white > 1000)
	{
		white = 1000;
	}

	__HAL_TIM_SET_COMPARE(&LED_PWM_FAST_TIMER, LED_R_CH_FAST, red);
	__HAL_TIM_SET_COMPARE(&LED_PWM_FAST_TIMER, LED_G_CH_FAST, green);
	__HAL_TIM_SET_COMPARE(&LED_PWM_FAST_TIMER, LED_B_CH_FAST, blue);
	__HAL_TIM_SET_COMPARE(&LED_PWM_FAST_TIMER, LED_W_CH_FAST, white);
}



void LED_PWM_SetSlowPWM(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	if(red > 1000)
	{
		red = 1000;
	}
	if(green > 1000)
	{
		green = 1000;
	}
	if(blue > 1000)
	{
		blue = 1000;
	}
	if(white > 1000)
	{
		white = 1000;
	}

	__HAL_TIM_SET_COMPARE(&LED_PWM_SLOW_TIMER, LED_R_CH_SLOW, red);
	__HAL_TIM_SET_COMPARE(&LED_PWM_SLOW_TIMER, LED_G_CH_SLOW, green);
	__HAL_TIM_SET_COMPARE(&LED_PWM_SLOW_TIMER, LED_B_CH_SLOW, blue);
	__HAL_TIM_SET_COMPARE(&LED_PWM_SLOW_TIMER, LED_W_CH_SLOW, white);
}



inline uint16_t MapRounded(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
	return (uint16_t)(((uint32_t)(x - in_min) * (uint32_t)(out_max - out_min) + (uint32_t)((in_max - in_min) >> 1)) / (uint32_t)(in_max - in_min)) + out_min;
}
