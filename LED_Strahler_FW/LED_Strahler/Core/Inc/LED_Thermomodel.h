/*
 * LED_Thermomodel.h
 *
 *  Created on: 03.11.2019
 *      Author: KaiLi
 */

#ifndef INC_LED_THERMOMODEL_H_
#define INC_LED_THERMOMODEL_H_



#define THERMOMODEL_PERIOD			100

#define FAN_MIN_PWM					300
#define FAN_MAX_PWM					1000

#define THERMOMODEL_FAN_MIN_TEMP	30.0
#define THERMOMODEL_FAN_MAX_TEMP	45.0
#define THERMOMODEL_FAN_HYSTERESIS	2.0

#define THERMOMODEL_FAN_MIN_ON_TEMP	(THERMOMODEL_FAN_MIN_TEMP + THERMOMODEL_FAN_HYSTERESIS)



void LED_Thermomodel_Init(void);
void LED_Thermomodel_Handle(void);
float LED_Thermomodel_GetTemp(void);

#endif /* INC_LED_THERMOMODEL_H_ */
