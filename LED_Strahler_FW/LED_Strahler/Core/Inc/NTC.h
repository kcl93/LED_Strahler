/*
 * NTC.h
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#ifndef INC_NTC_H_
#define INC_NTC_H_

#include "stm32f0xx_hal.h"
#include "main.h"



#define NTC_ADC_HANDLE		hadc



void NTC_Init(void);
float NTC_Get_Temp(void);


#endif /* INC_NTC_H_ */
