/*
 * LED_NRF24L01.c
 *
 *  Created on: 02.11.2019
 *      Author: KaiLi
 */

#include "LED_NRF24L01.h"
#include "LED_StateMachine.h"
#include "LED_Thermomodel.h"
#include "stm32f0xx_it.h"
#include "LED_NRF_Protocol.h"




//Functions to handle command
void Exec_PingRequest(struct NRF24L01_PingRequest packet);
void Exec_SetGroup(struct NRF24L01_SetGroup packet);
void Exec_SetTimebase(struct NRF24L01_SetTimebase packet);
void Exec_SetRGB(struct NRF24L01_SetRGB packet);
void Exec_SetRGBW(struct NRF24L01_SetRGBW packet);
void Exec_SetHSV(struct NRF24L01_SetHSV packet);
void Exec_StrobeRGB(struct NRF24L01_StrobeRGB packet);
void Exec_StrobeRGBW(struct NRF24L01_StrobeRGBW packet);
void Exec_StrobeHSV(struct NRF24L01_StrobeHSV packet);
void Exec_FadeHue(struct NRF24L01_FadeHue packet);
void Exec_FadeSaturation(struct NRF24L01_FadeSaturation packet);
void Exec_FadeValue(struct NRF24L01_FadeValue packet);
void Exec_GetTempertaure(struct NRF24L01_GetTemperature packet);



/* Own address */
uint32_t OwnAddress;

/* Own group */
uint8_t OwnGroup = 0;

/* NRF transmission status */
NRF24L01_IRQ_t NRF_IRQ;

// Timestamp is used as UUID
const char* timestamp = __TIMESTAMP__;



void LED_NRF24L01_Init(void)
{
	//Wrapper to char array containing the compilation timestamp
	uint32_t *wrapper = (uint32_t*)timestamp;

	OwnGroup = 0;

	//Init SPI interface
	NRF24L01_SPI_Send(0x55);

	NRF24L01_Init(LED_NRF24L01_CHANNEL, LED_NRF24L01_PAYLOAD); // Do basic init

	NRF24L01_SetRF(LED_NRF24L01_DATARATE, LED_NRF24L01_OUTPUT_POWER); /* Set RF settings */

	NRF24L01_CE_LOW;
	OwnAddress = LED_NRF24L01_BASE_ADDR;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, (uint8_t*)&OwnAddress, 4);	//Setup transmit address
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, (uint8_t*)&OwnAddress, 4); //Store main broadcast receive address
	OwnAddress = LED_NRF24L01_BROADCAST_ADDR;	//Setup pipe 0 for RX
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, (uint8_t*)&OwnAddress, 4); //Store main broadcast receive address
	NRF24L01_CE_HIGH;

	/* Go to RX mode */
	NRF24L01_PowerUpRx();

	//Calculate own unique address
	OwnAddress = wrapper[0] ^ wrapper[1] ^ wrapper[2] ^ wrapper[3] ^ wrapper[4] ^ wrapper[5];	//Calculate own address
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
	while (status & (1 << NRF24L01_RX_DR))
	{
		/* Get data from NRF24L01+ */
		NRF24L01_GetData(packet.Data);

		//Handle data
		switch(packet.CMD)
		{
			case CMD_PINGREQUEST: // Answer with own address for four times with "random" delays in between
				Exec_PingRequest(packet.PingRequest);
				break;

			case CMD_SETGROUP:
				Exec_SetGroup(packet.SetGroup);
				break;

			case CMD_SETTIMEBASE:
				Exec_SetTimebase(packet.SetTimebase);
				break;

			case CMD_SETRGB:
				Exec_SetRGB(packet.SetRGB);
				break;

			case CMD_SETRGBW:
				Exec_SetRGBW(packet.SetRGBW);
				break;

			case CMD_SETHSV:
				Exec_SetHSV(packet.SetHSV);
				break;

			case CMD_STROBERGB:
				Exec_StrobeRGB(packet.StrobeRGB);
				break;

			case CMD_STROBERGBW:
				Exec_StrobeRGBW(packet.StrobeRGBW);
				break;

			case CMD_STROBEHSV:
				Exec_StrobeHSV(packet.StrobeHSV);
				break;

			case CMD_FADEHUE:
				Exec_FadeHue(packet.FadeHue);
				break;

			case CMD_FADESATURATION:
				Exec_FadeSaturation(packet.FadeSaturation);
				break;

			case CMD_GETTEMPERATURE:
				Exec_GetTempertaure(packet.GetTemperature);
				break;

			default:
				break;
		}
		status = NRF24L01_Clear_Interrupts(); //Reads status register and then clears it
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
	//Send new data
	NRF24L01_Transmit(data);
}



inline void Exec_PingRequest(struct NRF24L01_PingRequest packet)
{
	((struct NRF24L01_PingAnswer*)&packet)->CMD = CMD_PINGREQUESTANSWER;
	((struct NRF24L01_PingAnswer*)&packet)->SlaveAddress = OwnAddress;
	//Wait for a random time before answering
	HAL_Delay((uint8_t)(OwnAddress));
	LED_NRF24L01_Send((uint8_t*)&packet);
	//Wait again for a random time before answering
	HAL_Delay((uint8_t)(OwnAddress >> 8));
	LED_NRF24L01_Send((uint8_t*)&packet);
	//Wait again for a random time before answering
	HAL_Delay((uint8_t)(OwnAddress >> 16));
	LED_NRF24L01_Send((uint8_t*)&packet);
	//Wait again for a random time before answering
	HAL_Delay((uint8_t)(OwnAddress >> 24));
	LED_NRF24L01_Send((uint8_t*)&packet);
	LED_NRF24L01_WaitTx(5);
	NRF24L01_PowerUpRx();
}



inline void Exec_SetGroup(struct NRF24L01_SetGroup packet)
{
	if(packet.SlaveAddress == OwnAddress)
	{
		OwnGroup = packet.GroupID;
	}
}



inline void Exec_SetTimebase(struct NRF24L01_SetTimebase packet)
{
	LED_UpdateTimebase(packet.Timebase);
}



inline void Exec_SetRGB(struct NRF24L01_SetRGB packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeRGB(packet.Red, packet.Green, packet.Blue);
	}
}



inline void Exec_SetRGBW(struct NRF24L01_SetRGBW packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeRGBW(packet.Red, packet.Green, packet.Blue, packet.White);
	}
}



inline void Exec_SetHSV(struct NRF24L01_SetHSV packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeHSV(packet.Hue, packet.Saturation, packet.Value);
	}
}



inline void Exec_StrobeRGB(struct NRF24L01_StrobeRGB packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeRGB(packet.Period, packet.Count, packet.Red, packet.Green, packet.Blue);
	}
}



inline void Exec_StrobeRGBW(struct NRF24L01_StrobeRGBW packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeRGBW(packet.Period, packet.Count, packet.Red, packet.Green, packet.Blue, packet.White);
	}
}



inline void Exec_StrobeHSV(struct NRF24L01_StrobeHSV packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeHSV(packet.Period, packet.Count, packet.Hue, packet.Saturation, packet.Value);
	}
}



inline void Exec_FadeHue(struct NRF24L01_FadeHue packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeHue(packet.Period, packet.Hue_Max, packet.Hue_Min, packet.Saturation, packet.Value);
	}
}



inline void Exec_FadeSaturation(struct NRF24L01_FadeSaturation packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeSaturation(packet.Period, packet.Hue, packet.Saturation_Max, packet.Saturation_Min, packet.Value);
	}
}



inline void Exec_FadeValue(struct NRF24L01_FadeValue packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeValue(packet.Period, packet.Hue, packet.Saturation, packet.Value_Max, packet.Value_Min);
	}
}



inline void Exec_GetTempertaure(struct NRF24L01_GetTemperature packet)
{
	((struct NRF24L01_GetTemperatureAnswer*)&packet)->CMD = CMD_GETTEMPERATUREANSWER;
	((struct NRF24L01_GetTemperatureAnswer*)&packet)->LED_Temperature = LED_Thermomodel_GetTemp();
	//Transmit answer once
	NRF24L01_Transmit((uint8_t*)&packet);
	while(NRF24L01_GetTransmissionStatus() == NRF24L01_Transmit_Status_Sending);
	NRF24L01_PowerUpRx();
}

