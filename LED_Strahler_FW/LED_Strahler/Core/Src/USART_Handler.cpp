/*
 * USART_Handler.cpp
 *
 *  Created on: Nov 9, 2019
 *      Author: KaiLi
 */


#include "USART_Handler.h"




USART_Handler::USART_Handler(UART_HandleTypeDef &_huart)
{
	this->huart = &_huart;

	//Register UART RX buffers
	HAL_UART_Receive_DMA(this->huart, this->rx_buffer, UART_RX_BUFFER_SIZE);
	rx_ptr = UART_RX_BUFFER_SIZE;
}



uint16_t USART_Handler::Available(void)
{
	return ((this->rx_ptr - (uint16_t)this->huart->hdmarx->Instance->CNDTR) % UART_RX_BUFFER_SIZE);
}


void USART_Handler::Write(uint8_t *arr, uint16_t cnt)
{
	while(this->Busy());

	if(cnt > UART_TX_BUFFER_SIZE)
	{
		cnt = UART_TX_BUFFER_SIZE;
	}

	do
	{
		cnt--;
		this->tx_buffer[cnt] = arr[cnt];
	}while(cnt != 0);

	HAL_UART_Transmit_DMA(this->huart, this->tx_buffer, (cnt+1));
}



uint8_t USART_Handler::Read(void)
{
	uint8_t ret;

	ret = this->rx_buffer[UART_RX_BUFFER_SIZE - rx_ptr];

	rx_ptr--;
	if(rx_ptr == 0)
	{
		rx_ptr = UART_RX_BUFFER_SIZE;
	}

	return ret;
}



uint16_t USART_Handler::Read(uint8_t *arr, uint16_t size)
{
	uint16_t cnt;

	if(this->rx_ptr == (uint16_t)this->huart->hdmarx->Instance->CNDTR) //No new data
	{
		return 0;
	}

	for(cnt = 0; ((cnt < UART_RX_BUFFER_SIZE) && (cnt < size)); cnt++)
	{
		arr[cnt] = this->Read();
	}

	return cnt;
}



void USART_Handler::Print(char *c)
{
	uint16_t i;

	while(this->Busy());

	for(i = 0; ((i < UART_TX_BUFFER_SIZE) && (c[i] != 0x00)); i++)
	{
		this->tx_buffer[i] = c[i];
	}

	HAL_UART_Transmit_DMA(this->huart, this->tx_buffer, i);
}



void USART_Handler::Println(char *c)
{
	uint16_t i;

	while(this->Busy());

	for(i = 0; ((i < (UART_TX_BUFFER_SIZE-1)) && (c[i] != 0x00)); i++)
	{
		this->tx_buffer[i] = c[i];
	}
	this->tx_buffer[i] = '\n';

	HAL_UART_Transmit_DMA(this->huart, this->tx_buffer, (i+1));
}


bool USART_Handler::Busy(void)
{
	if(/*(this->huart->hdmatx->State == HAL_DMA_STATE_BUSY) || */(huart->gState != HAL_UART_STATE_READY))
	{
		return true;
	}
	return false;
}
