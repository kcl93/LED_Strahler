/*
 * LED_StateMachine.h
 *
 *  Created on: Nov 3, 2019
 *      Author: KaiLi
 */

#ifndef INC_LED_STATEMACHINE_H_
#define INC_LED_STATEMACHINE_H_

#include "stm32f0xx_hal.h"


#define STATEMACHINE_PERIOD_MS		10

#define STROBE_ACTIVE_TICKS			2

//#define RGBW_LED
#define RGB_LED



void LED_StateMachine_Init(void);
void LED_StateMachine_Handle(void);
void LED_UpdateTimebase(uint32_t time);
void LED_SetModeRGB(uint16_t red, uint16_t green, uint16_t blue);
void LED_SetModeRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_SetModeHSV(uint16_t hue, uint16_t saturation, uint16_t value);
void LED_SetModeStrobeRGB(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue);
void LED_SetModeStrobeRGBW(uint8_t period, uint8_t count, uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_SetModeStrobeHSV(uint8_t period, uint8_t count, uint16_t hue, uint16_t saturation, uint16_t value);
void LED_SetModeFadeHue(uint16_t period, uint16_t hue_max, uint16_t hue_min, uint16_t saturation, uint16_t value);
void LED_SetModeFadeSaturation(uint16_t period, uint16_t hue, uint16_t saturation_max, uint16_t saturation_min, uint16_t value);
void LED_SetModeFadeValue(uint16_t period, uint16_t hue, uint16_t saturation_max, uint16_t value_max, uint16_t value_min);


#endif /* INC_LED_STATEMACHINE_H_ */
