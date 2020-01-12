/*
 * stm32_nrf24l01_hal.h
 *
 *  Created on: 02.11.2019
 *      Author: KaiLi
 */

#ifndef INC_STM32_NRF24L01_HAL_H_
#define INC_STM32_NRF24L01_HAL_H_

#include "stm32f3xx_hal.h"
#include "main.h"



/* SPI handle */
#define NRF24L01_SPI_HANDLE			hspi2

/* SPI chip enable pin */
#define NRF24L01_CSN_PORT			NRF_CS_GPIO_Port
#define NRF24L01_CSN_PIN			NRF_CS_Pin

/* Chip enable for transmitting */
#define NRF24L01_CE_PORT			NRF_CE_GPIO_Port
#define NRF24L01_CE_PIN				NRF_CE_Pin

/* Pins configuration */
#define NRF24L01_CE_LOW				HAL_GPIO_WritePin(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_PIN_RESET)
#define NRF24L01_CE_HIGH			HAL_GPIO_WritePin(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_PIN_SET)
#define NRF24L01_CSN_LOW			HAL_GPIO_WritePin(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, GPIO_PIN_RESET)
#define NRF24L01_CSN_HIGH			HAL_GPIO_WritePin(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, GPIO_PIN_SET)



uint8_t NRF24L01_SPI_Send(uint8_t data);
void NRF24L01_SPI_WriteMulti(uint8_t* dataOut, uint32_t count);
void NRF24L01_SPI_ReadMulti(uint8_t *dataIn, uint8_t dummy, uint32_t count);
void NRF24L01_SPI_SendMulti(uint8_t* dataOut, uint8_t* dataIn, uint32_t count);


#endif /* INC_STM32_NRF24L01_HAL_H_ */
