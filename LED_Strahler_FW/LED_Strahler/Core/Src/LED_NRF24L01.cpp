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
void Exec_PingRequest(struct NRF24L01_PingRequest &packet);
void Exec_SetGroup(struct NRF24L01_SetGroup &packet);
void Exec_SetTimebase(struct NRF24L01_SetTimebase &packet);
void Exec_SetRGB(struct NRF24L01_SetRGB &packet);
void Exec_SetRGBW(struct NRF24L01_SetRGBW &packet);
void Exec_SetHSV(struct NRF24L01_SetHSV &packet);
void Exec_StrobeRGB(struct NRF24L01_StrobeRGB &packet);
void Exec_StrobeRGBW(struct NRF24L01_StrobeRGBW &packet);
void Exec_StrobeHSV(struct NRF24L01_StrobeHSV &packet);
void Exec_FadeHue(struct NRF24L01_FadeHue &packet);
void Exec_FadeSaturation(struct NRF24L01_FadeSaturation &packet);
void Exec_FadeValue(struct NRF24L01_FadeValue &packet);
void Exec_GetTempertaure(struct NRF24L01_GetTemperature &packet);

uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len);


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
	OwnGroup = 0;

	//Init SPI interface
	NRF24L01_SPI_Send(0x55);

	NRF24L01_Init(LED_NRF24L01_CHANNEL, LED_NRF24L01_PAYLOAD); // Do basic init

	NRF24L01_SetRF(LED_NRF24L01_DATARATE, LED_NRF24L01_OUTPUT_POWER); /* Set RF settings */

	NRF24L01_CE_LOW;
	OwnAddress = LED_NRF24L01_BASE_ADDR;
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_TX_ADDR, (uint8_t*)&OwnAddress, 4);	//Setup transmit address
	OwnAddress = LED_NRF24L01_BROADCAST_ADDR;	//Setup pipe 0 for RX
	NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P0, (uint8_t*)&OwnAddress, 4); //Store main broadcast receive address
	//NRF24L01_WriteRegisterMulti(NRF24L01_REG_RX_ADDR_P1, (uint8_t*)&OwnAddress, 4); //Store main broadcast receive address
	NRF24L01_CE_HIGH;

	/* Go to RX mode */
	NRF24L01_PowerUpRx();

	//Calculate own unique address
	OwnAddress = rc_crc32(0, timestamp, 20);	//Calculate own address
	OwnAddress = 0x11111111;
	//OwnAddress = 0x22222222;
}


uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;

	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0)
	{
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++)
		{
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++)
			{
				if (rem & 1)
				{
					rem >>= 1;
					rem ^= 0xedb88320;
				}
				else
				{
					rem >>= 1;
				}
			}
			table[i] = rem;
		}
		have_table = 1;
	}

	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++)
	{
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}

	return ~crc;
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
	union NRF24L01_DataPacket packet = {0};
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

			case CMD_FADEVALUE:
				Exec_FadeValue(packet.FadeValue);
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
		status = NRF24L01_Clear_Interrupts();
		if (!NRF24L01_CHECK_BIT(status, NRF24L01_TX_FULL))
		{
			break; //New data can be sent once there is some free room in the TX FIFO :)
		}
	}
}


void LED_NRF24L01_Send(uint8_t* data)
{
	uint32_t starttime = HAL_GetTick();

	//Wait for previous transfer to finish or timeout
	LED_NRF24L01_WaitTx(5);

	//Send new data
	NRF24L01_Transmit(data);

	//Delay after sending so that data is actually send (don't know why this is neccessary)
	while(((HAL_GetTick() - starttime) < 2) && (HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port, NRF_IRQ_Pin) != 0));
}



inline void Exec_PingRequest(struct NRF24L01_PingRequest &packet)
{
	((struct NRF24L01_PingAnswer*)&packet)->CMD = CMD_PINGREQUESTANSWER;
	((struct NRF24L01_PingAnswer*)&packet)->SlaveAddress = OwnAddress;
	//Wait for a random time before answering
	HAL_Delay(((NRF24L01_DataPacket*)&packet)->Data[1]);
	LED_NRF24L01_Send(((NRF24L01_DataPacket*)&packet)->Data);
	//Wait again for a random time before answering
	HAL_Delay(((NRF24L01_DataPacket*)&packet)->Data[2]);
	LED_NRF24L01_Send(((NRF24L01_DataPacket*)&packet)->Data);
	//Wait again for a random time before answering
	HAL_Delay(((NRF24L01_DataPacket*)&packet)->Data[3]);
	LED_NRF24L01_Send(((NRF24L01_DataPacket*)&packet)->Data);
	//Wait again for a random time before answering
	HAL_Delay(((NRF24L01_DataPacket*)&packet)->Data[4]);
	LED_NRF24L01_Send(((NRF24L01_DataPacket*)&packet)->Data);
	LED_NRF24L01_WaitTx(5);
	NRF24L01_PowerUpRx();
}



inline void Exec_SetGroup(struct NRF24L01_SetGroup &packet)
{
	if(packet.SlaveAddress == OwnAddress)
	{
		OwnGroup = packet.GroupID;
	}
}



inline void Exec_SetTimebase(struct NRF24L01_SetTimebase &packet)
{
	LED_UpdateTimebase(packet.Timebase);
}



inline void Exec_SetRGB(struct NRF24L01_SetRGB &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeRGB(packet.Red, packet.Green, packet.Blue);
	}
}



inline void Exec_SetRGBW(struct NRF24L01_SetRGBW &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeRGBW(packet.Red, packet.Green, packet.Blue, packet.White);
	}
}



inline void Exec_SetHSV(struct NRF24L01_SetHSV &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeHSV(packet.Hue, packet.Saturation, packet.Value);
	}
}



inline void Exec_StrobeRGB(struct NRF24L01_StrobeRGB &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeRGB(packet.Period, packet.Count, packet.Red, packet.Green, packet.Blue);
	}
}



inline void Exec_StrobeRGBW(struct NRF24L01_StrobeRGBW &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeRGBW(packet.Period, packet.Count, packet.Red, packet.Green, packet.Blue, packet.White);
	}
}



inline void Exec_StrobeHSV(struct NRF24L01_StrobeHSV &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeStrobeHSV(packet.Period, packet.Count, packet.Hue, packet.Saturation, packet.Value);
	}
}



inline void Exec_FadeHue(struct NRF24L01_FadeHue &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeHue(packet.Period, packet.Hue_Max, packet.Hue_Min, packet.Saturation, packet.Value);
	}
}



inline void Exec_FadeSaturation(struct NRF24L01_FadeSaturation &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeSaturation(packet.Period, packet.Hue, packet.Saturation_Max, packet.Saturation_Min, packet.Value);
	}
}



inline void Exec_FadeValue(struct NRF24L01_FadeValue &packet)
{
	if((packet.GroupID == 0) || (packet.GroupID == OwnGroup))
	{
		LED_SetModeFadeValue(packet.Period, packet.Hue, packet.Saturation, packet.Value_Max, packet.Value_Min);
	}
}



inline void Exec_GetTempertaure(struct NRF24L01_GetTemperature &packet)
{
	if(packet.SlaveAddress == OwnAddress)
	{
		((struct NRF24L01_GetTemperatureAnswer*)&packet)->CMD = CMD_GETTEMPERATUREANSWER;
		((struct NRF24L01_GetTemperatureAnswer*)&packet)->LED_Temperature = LED_Thermomodel_GetTemp();
		//Wait for 2ms to be sure that the base is able to receive data
		HAL_Delay(2);
		//Transmit answer once
		LED_NRF24L01_Send((uint8_t*)&packet);
		LED_NRF24L01_WaitTx(5);
		NRF24L01_PowerUpRx();
	}
}

