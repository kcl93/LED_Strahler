/*
 * stm32_nrf24l01_hal.c
 *
 *  Created on: 02.11.2019
 *      Author: KaiLi
 */


#include "stm32_nrf24l01_hal.h"




extern SPI_HandleTypeDef NRF24L01_SPI_HANDLE;



uint8_t NRF24L01_SPI_Send(uint8_t data)
{
	while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_TXE));
	*(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR = data;
	while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_RXNE));
	data = *(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR;

	return data;
}



void NRF24L01_SPI_WriteMulti(uint8_t* dataOut, uint32_t count)
{
	while(count > 0)
	{
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_TXE));
		*(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR = *dataOut;
		dataOut++;
		count--;
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_RXNE));
		*(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR;
	}
}



void NRF24L01_SPI_ReadMulti(uint8_t *dataIn, uint8_t dummy, uint32_t count)
{
	while(count > 0)
	{
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_TXE));
		*(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR = 0;
		count--;
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_RXNE));
		*dataIn = *(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR;
		dataIn++;
	}
}


void NRF24L01_SPI_SendMulti(uint8_t* dataOut, uint8_t* dataIn, uint32_t count)
{
	while(count > 0)
	{
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_TXE));
		*(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR = *dataOut;
		dataOut++;
		count--;
		while(!(NRF24L01_SPI_HANDLE.Instance->SR & SPI_SR_RXNE));
		*dataIn = *(volatile uint8_t *)&NRF24L01_SPI_HANDLE.Instance->DR;
		dataIn++;
	}
}


