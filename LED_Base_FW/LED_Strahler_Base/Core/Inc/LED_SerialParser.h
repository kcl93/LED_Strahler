/*
 * LED_SerialParser.h
 *
 *  Created on: 11.11.2019
 *      Author: KaiLi
 */

#ifndef INC_LED_SERIALPARSER_H_
#define INC_LED_SERIALPARSER_H_


#include "stm32f3xx_hal.h"
#include "USART_Handler.h"
#include "LED_NRF_Protocol.h"



#define SERIALPARSER_CMD_BUFFER_SIZE	64




class LED_SerialParser
{
private:
	USART_Handler *uart;
	char cmd_buf[SERIALPARSER_CMD_BUFFER_SIZE];
	uint8_t cmd_buf_index;
	NRF24L01_DataPacket cmd;
	bool cmd_available;
	uint32_t address;

	void Decode(void);

public:
	LED_SerialParser(USART_Handler *_uart);

	void Parse(void);
	bool Available(void);
	void ReadCMD(NRF24L01_DataPacket *_cmd, uint32_t *_address);
};


#endif /* INC_LED_SERIALPARSER_H_ */
