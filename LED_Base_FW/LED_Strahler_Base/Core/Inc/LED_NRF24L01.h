/*
 * LED_NRF24L01.h
 *
 *  Created on: 02.11.2019
 *      Author: KaiLi
 */

#ifndef INC_LED_NRF24L01_H_
#define INC_LED_NRF24L01_H_

#include "stm32_nrf24l01.h"



void LED_NRF24L01_Init(void);
void LED_NRF24L01_IRQ(void);
void LED_NRF24L01_WaitTx(uint8_t timeout);
void LED_NRF24L01_Send(uint8_t* data, uint32_t address);

#endif /* INC_LED_NRF24L01_H_ */
