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


void LED_CopyRGB(uint16_t red, uint16_t green, uint16_t blue);
void LED_CopyRGW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_CopyHSV(uint16_t hue, uint16_t saturation, uint16_t value);
void SetHSV(uint16_t H, uint16_t S, uint16_t V);


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


uint16_t LED_Hue, LED_Saturation, LED_Value = 0;
uint16_t LED_Red, LED_Green, LED_Blue, LED_White = 0;
uint16_t LED_Fade_Min, LED_Fade_Max = 0;
uint32_t LED_Timebase = 0;
uint8_t Strobe_Period = 0;
uint8_t Strobe_State = 0;
uint8_t Strobe_Count = 0;
uint16_t Fade_Period = 0;
uint16_t Fade_State = 0;
USART_Handler *Serial = 0;
char buf[32];



void LED_StateMachine_Init(void)
{
	Serial = new USART_Handler(huart2);
	LED_Timebase = HAL_GetTick();

	LED_PWM_Init();

	LED_NRF24L01_Init(); //Setup RF-Interface

	LED_SetModeFadeHue(1000, 65535, 0, 65535, 1000);
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
				else if(Strobe_State >= Strobe_Period)
				{
					Strobe_Count--;
					Strobe_State = 0;
					LED_StateMachine_Handle();
				}
			}
			break;

		case LED_Mode_FadeHue:
			LED_PWM_SetBoost(0); //Deactivate boost
			LED_Hue = (uint16_t)((65535 * (uint32_t)Fade_State)/(uint32_t)Fade_Period) + LED_Fade_Max;
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			if(Fade_State >= Fade_Period)
			{
				Fade_State = 0;
			}
			else
			{
				Fade_State++;
			}
			break;

		case LED_Mode_FadeSaturation:
			LED_PWM_SetBoost(0); //Deactivate boost
			LED_Saturation = (uint16_t)(((uint32_t)(LED_Fade_Max - LED_Fade_Min) * (uint32_t)Fade_State)/(uint32_t)Fade_Period) + LED_Fade_Min;
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			if(Fade_State >= Fade_Period)
			{
				Fade_State = 0;
			}
			else
			{
				Fade_State++;
			}
			break;

		case LED_Mode_FadeValue:
			LED_PWM_SetBoost(0); //Deactivate boost
			LED_Value = (uint16_t)(((uint32_t)(LED_Fade_Max - LED_Fade_Min) * (uint32_t)Fade_State)/(uint32_t)Fade_Period) + LED_Fade_Min;
			SetHSV(LED_Hue, LED_Saturation, LED_Value); //Calculate RGB from HSV
			LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
			LED_PWM_SetRGBW(LED_Red, LED_Green, LED_Blue, LED_White); //Calculate
			if(Fade_State >= Fade_Period)
			{
				Fade_State = 0;
			}
			else
			{
				Fade_State++;
			}
			break;

		case LED_Mode_Idle:
			//Do nothing
			break;

		default: //Unknown state! disable lights and go to idle mode
			LED_CopyRGW(0, 0, 0, 0);
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
	#if defined(RGBW_LED)
		LED_White = LED_Red;
		if(LED_Green < LED_White)
		{
			LED_White = LED_Green;
		}
		if(LED_Blue < LED_White)
		{
			LED_White = LED_Blue;
		}

		LED_Red   -= LED_White;
		LED_Green -= LED_White;
		LED_Blue  -= LED_White;
	#elif defined(RGB_LED)
		LED_Red = red;
		LED_Green = green;
		LED_Blue = blue;
	#endif
}



inline void LED_CopyRGW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	#if defined(RGBW_LED)
		LED_Red = red;
		LED_Green = green;
		LED_Blue = blue;
		LED_White = white;
	#elif defined(RGB_LED)
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
	Serial->Println("Set RGB...");
}



void LED_SetModeRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	LED_CopyRGW(red, green, blue, white);
	LED_mode = LED_Mode_Constant;
	Serial->Println("Set RGBW...");
}



void LED_SetModeHSV(uint16_t hue, uint16_t saturation, uint16_t value)
{
	LED_CopyHSV(hue, saturation, value);
	SetHSV(LED_Hue, LED_Saturation, LED_Value);
	LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
	LED_mode = LED_Mode_Constant;
	Serial->Println("Set HSV...");
}



void LED_SetModeStrobeRGB(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	LED_CopyRGB(red, green, blue);
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	Serial->Println("Strobing RGB...");
}



void LED_SetModeStrobeRGBW(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue, uint16_t white)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	LED_CopyRGW(red, green, blue, white);
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	Serial->Println("Strobing RGBW...");
}



void LED_SetModeStrobeHSV(uint8_t period, uint8_t count, uint16_t hue, uint16_t saturation, uint16_t value)
{
	if(period < 2*STROBE_ACTIVE_TICKS)
	{
		period = 2*STROBE_ACTIVE_TICKS;
	}
	Strobe_Period = period;
	LED_CopyHSV(hue, saturation, value);
	SetHSV(LED_Hue, LED_Saturation, LED_Value);
	LED_CopyRGB(LED_Red, LED_Green, LED_Blue); //Convert RGB to RGBW
	Strobe_State = 0;
	LED_mode = LED_Mode_Strobe;
	Serial->Println("Strobing HSV...");
}



void LED_SetModeFadeHue(uint16_t period, uint16_t hue_max, uint16_t hue_min, uint16_t saturation, uint16_t value)
{
	LED_CopyHSV(hue_max, saturation, value);
	LED_Fade_Max = hue_max;
	LED_Fade_Min = hue_min;
	Fade_Period = period;
	Fade_State = 0;
	LED_mode = LED_Mode_FadeHue;
	Serial->Println("Fading hue...");
}



void LED_SetModeFadeSaturation(uint16_t period, uint16_t hue, uint16_t saturation_max, uint16_t saturation_min, uint16_t value)
{
	LED_CopyHSV(hue, saturation_max, value);
	LED_Fade_Max = saturation_max;
	LED_Fade_Min = saturation_min;
	Fade_Period = period;
	Fade_State = 0;
	LED_mode = LED_Mode_FadeSaturation;
	Serial->Println("Fading saturation...");
}



void LED_SetModeFadeValue(uint16_t period, uint16_t hue, uint16_t saturation, uint16_t value_max, uint16_t value_min)
{
	LED_CopyHSV(hue, saturation, value_max);
	LED_Fade_Max = value_max;
	LED_Fade_Min = value_min;
	Fade_Period = period;
	Fade_State = 0;
	LED_mode = LED_Mode_FadeSaturation;
	Serial->Println("Fading value...");
}



void SetHSV(uint16_t H, uint16_t S, uint16_t V)
{
	uint16_t X, Y;

	if ((H >= 0) && (H < 10922))
	{
		X = H;
		Y = (((uint32_t)V) * (uint32_t)X) / 10922;
		LED_Red = V;
		LED_Green = Y;
		LED_Blue = 0;
	}
	else if ((H >= 10922) && (H < 21845))
	{
		X = H - 10922;
		Y = (((uint32_t)V) * (uint32_t)X) / 10923;
		LED_Red = V - Y;
		LED_Green = V;
		LED_Blue = 0;
	}
	else if ((H >= 21845) && (H < 32767))
	{
		X = H - 21845;
		Y = (((uint32_t)V) * (uint32_t)X) / 10922;
		LED_Red = 0;
		LED_Green = V;
		LED_Blue = Y;
	}
	else if ((H >= 32767) && (H < 43690))
	{
		X = H - 32767;
		Y = (((uint32_t)V) * (uint32_t)X) / 10923;
		LED_Red = 0;
		LED_Green = V - Y;
		LED_Blue = V;
	}
	else if ((H >= 43690) && (H < 54612))
	{
		X = H - 43690;
		Y = (((uint32_t)V) * (uint32_t)X) / 10922;
		LED_Red = Y;
		LED_Green = 0;
		LED_Blue = V;
	}
	else
	{
		X = H - 54612;
		Y = (((uint32_t)V) * (uint32_t)X) / 10923;
		LED_Red = V;
		LED_Green = 0;
		LED_Blue = V - Y;
	}
}
