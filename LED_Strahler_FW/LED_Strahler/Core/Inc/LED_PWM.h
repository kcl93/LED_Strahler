/*
 * LED_PWM.h
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#ifndef INC_LED_PWM_H_
#define INC_LED_PWM_H_

#include "stm32f0xx_hal.h"
#include "main.h"


#define LED_PWM_FAST_TIMER			htim1
#define LED_PWM_SLOW_TIMER			htim3

#define LED_PWM_BOOST_PEAK_CURRENT	1000
#define LED_PWM_STD_PEAK_CURRENT	600
#define LED_PWM_ANALOG_MIN_CURRENT	400

#define LED_PWM_ANALOG_VOLT_MIN		250
#define LED_PWM_ANALOG_VOLT_MAX		2500

#define LED_PWM_BOOST_TIMEOUT		100



void LED_PWM_Init(void);
void LED_PWM_SetRGBW(uint16_t red, uint16_t green, uint16_t blue, uint16_t white);
void LED_PWM_SetBoost(uint8_t active);
void LED_PWM_CheckBoost(void);

#endif /* INC_LED_PWM_H_ */
