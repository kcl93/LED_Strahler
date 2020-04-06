/*
 * LED_Base.cpp
 *
 *  Created on: Nov 9, 2019
 *      Author: KaiLi
 */


#include "LED_Base.h"
#include "USART_Handler.h"
#include "LED_SerialParser.h"
#include "LED_NRF24L01.h"
#include <stdio.h>



void ExecPingRequest(NRF24L01_DataPacket *cmd);
void ExecGetTemperature(NRF24L01_DataPacket *cmd);


extern UART_HandleTypeDef BT_UART_HANDLE;
extern UART_HandleTypeDef PC_UART_HANDLE;


USART_Handler *BT_uart;
USART_Handler *PC_uart;
LED_SerialParser *BT_parser;
LED_SerialParser *PC_parser;
uint32_t sync_time;


void LED_Base_Setup(void)
{
	BT_uart = new USART_Handler(BT_UART_HANDLE);
	PC_uart = new USART_Handler(PC_UART_HANDLE);

	BT_parser = new LED_SerialParser(BT_uart);
	PC_parser = new LED_SerialParser(PC_uart);

	LED_NRF24L01_Init();

	sync_time = HAL_GetTick() - TIMEBASE_SYNC_PERIOD;
}



void LED_Base_Handle(void)
{
	NRF24L01_DataPacket cmd = {0};
	uint32_t address = 0;
	cmd.CMD = CMD_IGNORE;

	//Parse inputs for new data
	BT_parser->Parse();
	PC_parser->Parse();

	//Check for new data
	if(BT_parser->Available() == true)
	{
		BT_parser->ReadCMD(&cmd, &address);
	}
	else if(PC_parser->Available() == true)
	{
		PC_parser->ReadCMD(&cmd, &address);
	}

	//Check IRQ pin
	if(HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) == 0)
	{
		LED_NRF24L01_IRQ();
	}

	//Check if a new command was received
	if(cmd.CMD == CMD_IGNORE)
	{
		//No new command received! Handle timebase sync commands instead
		if((HAL_GetTick() - sync_time) >= TIMEBASE_SYNC_PERIOD)
		{
			sync_time = HAL_GetTick();
			cmd.SetTimebase.CMD = CMD_SETTIMEBASE;
			cmd.SetTimebase.Timebase = sync_time;
			LED_NRF24L01_Send(cmd.Data, LED_NRF24L01_BROADCAST_ADDR);
		}
		return;
	}

	//Execute new command
	switch(cmd.CMD)
	{
	case CMD_PINGREQUEST:
		ExecPingRequest(&cmd);
		break;

	case CMD_GETTEMPERATURE:
		ExecGetTemperature(&cmd);
		break;

	default:
		LED_NRF24L01_Send(cmd.Data, address);
		break;
	}
}



inline void ExecPingRequest(NRF24L01_DataPacket *cmd)
{
	uint32_t time;
	char buf[16];

	cmd->CMD = CMD_PINGREQUEST;
	LED_NRF24L01_Send(cmd->Data, LED_NRF24L01_BROADCAST_ADDR); //Ping all slaves to see which are present

	//Wait for transfer to finish or timeout
	LED_NRF24L01_WaitTx(5);

	/* Go to RX mode */
	NRF24L01_PowerUpRx();

	time = HAL_GetTick();
	while((HAL_GetTick() - time) <= PING_TIMEOUT)
	{
		if(HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) == 0) //Wait for data being received
		{
			if(NRF24L01_Clear_Interrupts() & (1 << NRF24L01_RX_DR))
			{
				/* Get data from NRF24L01+ */
				NRF24L01_GetData(cmd->Data);

				if(cmd->CMD == CMD_PINGREQUESTANSWER)
				{
					//Print answer to both serials
					sprintf(buf, "P: %u\n", (unsigned int)cmd->PingAnswer.SlaveAddress);
					//BT_uart->Print(buf);
					PC_uart->Print(buf);
				}
			}
		}
	}

	//Check if more data is available in the FIFO and get the rest
	while(!(NRF24L01_ReadRegister(NRF24L01_REG_FIFO_STATUS) & (1 << NRF24L01_RX_EMPTY)))
	{
		/* Get data from NRF24L01+ */
		NRF24L01_GetData(cmd->Data);

		if(cmd->CMD == CMD_PINGREQUESTANSWER)
		{
			//Print answer to both serials
			sprintf(buf, "P: %u\n", (unsigned int)cmd->PingAnswer.SlaveAddress);
			//BT_uart->Print(buf);
			PC_uart->Print(buf);
		}
	}

	/* Go to TX mode */
	NRF24L01_PowerUpTx();
}



inline void ExecGetTemperature(NRF24L01_DataPacket *cmd)
{
	uint32_t time;
	char buf[16];

	cmd->CMD = CMD_GETTEMPERATURE;
	LED_NRF24L01_Send(cmd->Data, LED_NRF24L01_BROADCAST_ADDR); //Ask for temperature

	//Wait for transfer to finish or timeout
	LED_NRF24L01_WaitTx(5);

	/* Go to RX mode */
	NRF24L01_PowerUpRx();

	time = HAL_GetTick();
	while((HAL_GetTick() - time) <= GET_INFO_TIMEOUT)
	{
		if(HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) == 0) //Wait for data being received
		{
			if(NRF24L01_Clear_Interrupts() & (1 << NRF24L01_RX_DR))
			{
				/* Get data from NRF24L01+ */
				NRF24L01_GetData(cmd->Data);

				if(cmd->CMD == CMD_GETTEMPERATUREANSWER)
				{
					//Print answer to both serials
					sprintf(buf, "T: %.1f\n", cmd->GetTemperatureAnswer.LED_Temperature);
					//BT_uart->Print(buf);
					PC_uart->Print(buf);
					break;
				}
			}
		}
	}

	/* Go to TX mode */
	NRF24L01_PowerUpTx();
}
