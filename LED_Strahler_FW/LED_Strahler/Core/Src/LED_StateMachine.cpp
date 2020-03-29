/*
 * LED_StateMachine.c
 *
 *  Created on: Nov 3, 2019
 *      Author: KaiLi
 */

#include "LED_StateMachine.h"
#include "LED_PWM.h"
#include "LED_NRF24L01.h"
#include "USART_Handler.h"


#if (defined(RGB_LED) && defined(RGBW_LED))
	#error Only define one type of LED!
#endif
#if (!defined(RGB_LED) && !defined(RGBW_LED))
	#error Please define which type of LED is present!
#endif


enum LED_Mode_Enum
{
	LED_Mode_Constant,
	LED_Mode_Strobe,
	LED_Mode_FadeHue,
	LED_Mode_FadeSaturation,
	LED_Mode_FadeValue,
	LED_Mode_Idle
}LED_mode = LED_Mode_Idle;


extern UART_HandleTypeDef huart2;


void LED_CopyRGB(uint16_t red, uint16_t green, uint16_t blue);
void LED_CopyRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_CopyHSV(uint16_t hue, uint16_t saturation, uint16_t value);
void SetHSV(uint16_t H, uint16_t S, uint16_t V);
void FadeLED(uint16_t *val);
void SetNewFadeLED(enum LED_Mode_Enum mode, uint16_t period);


uint16_t LED_Hue, LED_Saturation, LED_Value = 0;
uint16_t LED_Red, LED_Green, LED_Blue, LED_White = 0;
uint16_t LED_Fade_Min, LED_Fade_Max = 0;
uint32_t LED_Timebase = 0;
uint8_t Strobe_Period = 0;
uint8_t Strobe_State = 0;
uint8_t Strobe_Count = 0;
uint16_t Fade_Period = 1;
uint16_t Fade_State = 0;
bool FadeUp = true;
USART_Handler *Serial = 0;
char buf[32];



void LED_StateMachine_Init(void)
{
	Serial = new USART_Handler(huart2);
	LED_Timebase = HAL_GetTick();

	LED_PWM_Init();

	LED_NRF24L01_Init(); //Setup RF-Interface
}



void LED_StateMachine_Handle(void)
{
	LED_PWM_CheckBoost();

	if((HAL_GetTick() - LED_Timebase) > STATEMACHINE_PERIOD_MS)
	{
		LED_Timebase += STATEMACHINE_PERIOD_MS;

		//Disable interrupts
		__disable_irq();

		//LED statemachine
		switch(LED_mode)
		{
		case LED_Mode_Constant:
			LED_PWM_SetBoost(0); //Deactivate boost
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White);
			LED_mode = LED_Mode_Idle;
			break;

		case LED_Mode_Strobe:
			LED_PWM_SetBoost(1); //Activate boost
			if(Strobe_Count == 0) //Check if no more strobes should be output
			{
				LED_PWM_SetRGBW(0, 0, 0, 0);
				LED_mode = LED_Mode_Idle;
			}
			else
			{
				if(Strobe_State < STROBE_ACTIVE_TICKS)
				{
					LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White);
				}
				else if(Strobe_State < Strobe_Period)
				{
					LED_PWM_SetRGBW(0, 0, 0, 0);
				}
				if(Strobe_State >= Strobe_Period)
				{
					Strobe_Count--;
					Strobe_State = 0;
					LED_StateMachine_Handle();
				}
				else
				{
					Strobe_State++;
				}
			}
			break;

		case LED_Mode_FadeHue:
			LED_PWM_SetBoost(0); //Deactivate boost
			if((LED_Fade_Min == 0) && (LED_Fade_Max == 65535))
			{
				LED_Hue = (uint16_t)((65535 * (uint32_t)Fade_State)/((uint32_t)Fade_Period));
				if(Fade_State >= Fade_Period)
				{
					Fade_State = 0;
				}
				else
				{
					Fade_State++;
				}
			}
			else
			{
				FadeLED(&LED_Hue); //Fade LED hue
			}
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			break;

		case LED_Mode_FadeSaturation:
			LED_PWM_SetBoost(0); //Deactivate boost
			FadeLED(&LED_Saturation); //Fade LED saturation
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			break;

		case LED_Mode_FadeValue:
			LED_PWM_SetBoost(0); //Deactivate boost
			FadeLED(&LED_Value); //Fade LED value
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			break;

		case LED_Mode_Idle:
			//Do nothing
			break;

		default: //Unknown state! disable lights and go to idle mode
			LED_CopyRGBW(0, 0, 0, 0);
			LED_PWM_SetRGBW(0, 0, 0, 0);
			LED_mode = LED_Mode_Idle;
			break;
		}

		//Reenable interrupts
		__enable_irq();
	}
}



void LED_CopyRGB(uint16_t red, uint16_t green, uint16_t blue)
{
	#ifdef RGBW_LED
		LED_White = red;
		if(green < LED_White)
		{
			LED_White = green;
		}
		if(blue < LED_White)
		{
			LED_White = blue;
		}

		LED_Red   = red - LED_White;
		LED_Green = green - LED_White;
		LED_Blue  = blue - LED_White;
	#else //RGB LED
		LED_Red = red;
		LED_Green = green;
		LED_Blue = blue;
	#endif
}



inline void LED_CopyRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	#ifdef RGBW_LED
		LED_Red = red;
		LED_Green = green;
		LED_Blue = blue;
		LED_White = white;
	#else //RGB_LED
		LED_Red = ((((uint32_t)red) + (uint32_t)white) >> 1);
		LED_Green = ((((uint32_t)green) + (uint32_t)white) >> 1);
		LED_Blue = ((((uint32_t)blue) + (uint32_t)white) >> 1);
	#endif
}



inline void LED_CopyHSV(uint16_t hue, uint16_t saturation, uint16_t value)
{
	LED_Hue = hue;
	LED_Saturation = saturation;
	LED_Value = value;
}



void FadeLED(uint16_t *val)
{
	uint32_t TmpVal = (65535 * (uint32_t)Fade_State)/(uint32_t)Fade_Period;

	if(FadeUp == true)
	{
		if(TmpVal >= LED_Fade_Max)
		{
			if(TmpVal > 65535)
			{
				*val = 65535;
			}
			FadeUp = false;
			return;
		}
		Fade_State++;
	}
	else
	{
		if(TmpVal <= LED_Fade_Min)
		{
			if(TmpVal < 0)
			{
				*val = 0;
			}
			FadeUp = true;
			return;
		}
		Fade_State--;
	}

	*val = (uint16_t)TmpVal;
}



void SetNewFadeLED(enum LED_Mode_Enum mode, uint16_t period)
{
	uint32_t TmpVal;

	if(period == 0)
	{
		period = 1;
	}

	if(LED_mode == mode)
	{
		TmpVal = (65535 * (uint32_t)Fade_State)/(uint32_t)Fade_Period;
		if((FadeUp == true) && (TmpVal >= LED_Fade_Max))
		{
			if(TmpVal >= LED_Fade_Max)
			{
				FadeUp = false;
			}
		}
		else if(TmpVal <= LED_Fade_Min)
		{
			FadeUp = true;
		}
		if(period != Fade_Period)
		{
			Fade_State = ((uint32_t)Fade_State*(uint32_t)period)/(uint32_t)Fade_Period;
		}
	}
	else
	{
		LED_mode = mode;
		FadeUp = true;
		Fade_State = (uint16_t)(((uint32_t)LED_Fade_Min*(uint32_t)Fade_Period)/65535);
	}

	Fade_Period = period;
}



void LED_UpdateTimebase(uint32_t time)
{
	uint32_t tmp = LED_Timebase - uwTick;
	uwTick = time;
	LED_Timebase = uwTick + tmp;
}



void LED_SetModeRGB(uint16_t red, uint16_t green, uint16_t blue)
{
	LED_CopyRGB(red, green, blue);
	LED_mode = LED_Mode_Constant;
	//Serial->Println("Set RGB...");
}



void LED_SetModeRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	LED_CopyRGBW(red, green, blue, white);
	LED_mode = LED_Mode_Constant;
	//Serial->Println("Set RGBW...");
}



void LED_SetModeHSV(uint16_t hue, uint16_t saturation, uint16_t value)
{
	LED_CopyHSV(hue, saturation, value);
	SetHSV(LED_Hue, LED_Saturation, LED_Value);
	LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
	LED_mode = LED_Mode_Constant;
	//Serial->Println("Set HSV...");
}



void LED_SetModeStrobeRGB(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	Strobe_Count = count;
	LED_CopyRGB(red, green, blue);
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	//Serial->Println("Strobing RGB...");
}



void LED_SetModeStrobeRGBW(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	Strobe_Count = count;
	LED_CopyRGBW(red, green, blue, white);
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	//Serial->Println("Strobing RGBW...");
}



void LED_SetModeStrobeHSV(uint8_t period, uint8_t count, uint16_t hue, uint16_t saturation, uint16_t value)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	Strobe_Count = count;
	LED_CopyHSV(hue, saturation, value);
	SetHSV(LED_Hue, LED_Saturation, LED_Value);
	LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	//Serial->Println("Strobing HSV...");
}



void LED_SetModeFadeHue(uint16_t period, uint16_t hue_max, uint16_t hue_min, uint16_t saturation, uint16_t value)
{
	LED_CopyHSV(hue_max, saturation, value);
	LED_Fade_Max = hue_max;
	LED_Fade_Min = hue_min;
	SetNewFadeLED(LED_Mode_FadeHue, period);
	//Serial->Println("Fading hue...");
}



void LED_SetModeFadeSaturation(uint16_t period, uint16_t hue, uint16_t saturation_max, uint16_t saturation_min, uint16_t value)
{
	LED_CopyHSV(hue, saturation_max, value);
	LED_Fade_Max = saturation_max;
	LED_Fade_Min = saturation_min;
	SetNewFadeLED(LED_Mode_FadeSaturation, period);
	//Serial->Println("Fading saturation...");
}



void LED_SetModeFadeValue(uint16_t period, uint16_t hue, uint16_t saturation, uint16_t value_max, uint16_t value_min)
{
	LED_CopyHSV(hue, saturation, value_max);
	LED_Fade_Max = value_max;
	LED_Fade_Min = value_min;
	SetNewFadeLED(LED_Mode_FadeValue, period);
	//Serial->Println("Fading value...");
}



void SetHSV(uint16_t H, uint16_t S, uint16_t V)
{
	uint32_t region, remainder;
	uint16_t p, q, t;

	region = H / 10923;
	remainder = (H - (region * 10923)) * 6;

	p = ((uint32_t)V * (uint32_t)(65535 - S)) >> 16;
	q = ((uint32_t)V * (65535 - (((uint32_t)S * remainder) >> 16))) >> 16;
	t = ((uint32_t)V * (65535 - (((uint32_t)S * (65535 - remainder)) >> 16))) >> 16;

	switch (region)
	{
		case 0:
			LED_Red = V;
			LED_Green = t;
			LED_Blue = p;
			break;

		case 1:
			LED_Red = q;
			LED_Green = V;
			LED_Blue = p;
			break;

		case 2:
			LED_Red = p;
			LED_Green = V;
			LED_Blue = t;
			break;

		case 3:
			LED_Red = p;
			LED_Green = q;
			LED_Blue = V;
			break;

		case 4:
			LED_Red = t;
			LED_Green = p;
			LED_Blue = V;
			break;

		default:
			LED_Red = V;
			LED_Green = p;
			LED_Blue = q;
			break;
	}
}
