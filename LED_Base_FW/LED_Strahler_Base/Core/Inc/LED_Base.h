/*
 * LED_Base.h
 *
 *  Created on: Nov 9, 2019
 *      Author: KaiLi
 */

#ifndef INC_LED_BASE_H_
#define INC_LED_BASE_H_



#include "main.h"
#include "stm32f3xx_hal.h"




#define BT_UART_HANDLE			huart2
#define PC_UART_HANDLE			huart3

#define TIMEBASE_SYNC_PERIOD	10000	//Oscillators should have an error of ~+-1ms each 20s
#define PING_TIMEOUT			1100	//Theoretically all should answer within 4*256=1024ms
#define GET_INFO_TIMEOUT		20		//Slaves should answer within 20ms


void LED_Base_Setup(void);
void LED_Base_Handle(void);



#endif /* INC_LED_BASE_H_ */
