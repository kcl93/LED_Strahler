/*
 * LED_NRF24L01.c
 *
 *  Created on: 02.11.2019
 *      Author: KaiLi
 */

#include "LED_NRF24L01.h"
#include "stm32f3xx_it.h"
#include "LED_NRF_Protocol.h"
#include "stm32f3xx_hal.h"


/**
* The STM32 factory-programmed UUID memory.
* Three values of 32 bits each starting at this address
* Use like this: STM32_UUID[0], STM32_UUID[1], STM32_UUID[2]
*/
#define STM32_UUID ((uint32_t *)0x1FFF7A10)



/* Own address */
uint32_t OwnAddress;

/* Own group */
uint8_t OwnGroup = 0;

/* NRF transmission status */
NRF24L01_IRQ_t NRF_IRQ;



void LED_NRF24L01_Init(void)
{
	OwnGroup = 0;

	//Init SPI interface
	NRF24L01_SPI_Send(0x55);

	NRF24L01_Init(LED_NRF24L01_CHANNEL, LED_NRF24L01_PAYLOAD); // Do basic init

	NRF24L01_SetRF(LED_NRF24L01_DATARATE, LED_NRF24L01_OUTPUT_POWER); /* Set RF settings */

	NRF24L01_CE_LOW;
	OwnAddress = LED_NRF24L01_BROADCAST_ADDR;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, (uint8_t*)&OwnAddress, 4);	//Setup transmit address
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, (uint8_t*)&OwnAddress, 4);	//Setup receive address
	OwnAddress = LED_NRF24L01_BASE_ADDR;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, (uint8_t*)&OwnAddress, 4);	//Setup receive address
	NRF24L01_CE_HIGH;

	/* Go to RX mode */
	NRF24L01_PowerUpTx();
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == NRF_IRQ_Pin)
	{
		LED_NRF24L01_IRQ();
	}
}


void LED_NRF24L01_IRQ(void)
{
	union NRF24L01_DataPacket packet;
	uint8_t status;

	/* Read interrupts, If data is ready on NRF24L01+ */
	status = NRF24L01_Clear_Interrupts(); //Reads status register and then clears it
	if (status & (1 << NRF24L01_RX_DR))
	{
		/* Get data from NRF24L01+ */
		NRF24L01_GetData(packet.Data);
	}
	if(status & (1 << NRF24L01_TX_DS))
	{
		//Reset LED as data was sent
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	}
}


void LED_NRF24L01_WaitTx(uint8_t timeout)
{
	uint32_t starttime;
	uint8_t status;

	//Wait for previous transfer to finish or timeout
	starttime = HAL_GetTick();
	while((HAL_GetTick() - starttime) < timeout)
	{
		status = NRF24L01_GetStatus();
		if (!NRF24L01_CHECK_BIT(status, NRF24L01_TX_FULL))
		{
			break; //New data can be sent once there is some free room in the TX FIFO :)
		}
	}
}


void LED_NRF24L01_Send(uint8_t* data)
{
	//Wait for previous transfer to finish or timeout
	LED_NRF24L01_WaitTx(5);
	//Set LED that new data will be sent
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	//Send new data
	NRF24L01_Transmit(data);
}
