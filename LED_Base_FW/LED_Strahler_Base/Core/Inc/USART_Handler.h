/*
 * USART_Handler.h
 *
 *  Created on: Nov 9, 2019
 *      Author: KaiLi
 */

#ifndef INC_USART_HANDLER_H_
#define INC_USART_HANDLER_H_


#define UART_RX_BUFFER_SIZE		256
#define UART_TX_BUFFER_SIZE		256


#include "stm32f3xx_hal.h"



class USART_Handler
{
	private:
		uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
		uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
		UART_HandleTypeDef *huart;
		uint16_t rx_ptr;

	public:
		USART_Handler(UART_HandleTypeDef &_huart);
		uint16_t Available(void);
		void Write(uint8_t *arr, uint16_t cnt);
		inline void Write(uint8_t byte)
			{
				this->Write(&byte, 1);
			}
		uint8_t Read(void);
		uint16_t Read(uint8_t *arr, uint16_t size);
		void Print(char *c);
		void Println(char *c);
		bool Busy(void);

		inline void Print(const char *c)
			{
				this->Print((char*)c);
			}
		inline void Println(const char *c)
			{
				this->Println((char*)c);
			}
		inline void Println(void)
			{
				this->Print("\n");
			}
};




#endif /* INC_USART_HANDLER_H_ */
