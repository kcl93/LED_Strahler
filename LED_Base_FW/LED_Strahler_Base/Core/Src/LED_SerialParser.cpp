/*
 * LED_SerialParser.cpp
 *
 *  Created on: 11.11.2019
 *      Author: KaiLi
 */

#include "LED_SerialParser.h"
#include <stdio.h>




LED_SerialParser::LED_SerialParser(USART_Handler *_uart)
{
	uint8_t i;

	this->uart = _uart;

	for(i = 0; i < SERIALPARSER_CMD_BUFFER_SIZE; i++)
	{
		this->cmd_buf[i] = 0x00;
	}
	this->cmd_buf_index = 0;
}


void LED_SerialParser::Parse(void)
{
	char byte;

	while(this->uart->Available())
	{
		byte = this->uart->Read();
		if(this->cmd_buf_index == 0)
		{ //Search for valid start byte
			if(byte == 'G')
			{
				this->cmd_buf[0] = byte;
				this->cmd_buf_index = 1;
			}
		}
		else if(this->cmd_buf_index < SERIALPARSER_CMD_BUFFER_SIZE)
		{ //Parse new string until end of buffer or new line is reached
			if(byte == 'G')
			{ //Reset command as new start of CMD was received
				this->cmd_buf[0] = byte;
				this->cmd_buf_index = 1;
			}
			else
			{ //Fill command buffer with new characters
				this->cmd_buf[this->cmd_buf_index] = byte;
				if(byte == '\n')
				{ //Command seems to be finished
					this->cmd_buf[this->cmd_buf_index] = 0x00;
					this->Decode();
					this->cmd_buf_index = 0;
					break;
				}
				this->cmd_buf_index++;
			}
		}
		else
		{ //Buffer overflow! Drop current command buffer!
			this->cmd_buf_index = 0;
		}
	}
}


void LED_SerialParser::Decode(void)
{
	uint8_t ret = 0;
	unsigned int u32Tmp[8] = {0};
	this->cmd = {0};

	//Get command
	ret = sscanf(this->cmd_buf, "G%u %u %u %u %u %u %u %u", &u32Tmp[0], &u32Tmp[1], &u32Tmp[2], &u32Tmp[3], &u32Tmp[4], &u32Tmp[5], &u32Tmp[6], &u32Tmp[7]);
	if(ret == 0)
	{
		return; //Could not fetch command!
	}
	this->cmd.CMD = (uint8_t)u32Tmp[0];

	//Decode based on command
	switch(this->cmd.CMD)
	{
		case CMD_PINGREQUEST:
			break; //Nothing to do

		case CMD_SETGROUP:
			if(ret != 3)
			{
				return; //Error fetching command
			}
			this->cmd.SetGroup.SlaveAddress = u32Tmp[1];
			this->cmd.SetGroup.GroupID = (uint8_t)u32Tmp[2];
			break;

		/*case CMD_SETTIMEBASE:
			ret = sscanf(this->cmd_buf, "* %u", (unsigned int*)&this->cmd.SetTimebase.Timebase);
			if(ret != 2)
			{
				return; //Error fetching command
			}
			this->cmd.SetTimebase.Timebase = u32Tmp[1];
			break;*/

		case CMD_SETRGB:
			if(ret != 5)
			{
				return; //Error fetching command
			}
			this->cmd.SetRGB.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.SetRGB.Red = (uint16_t)u32Tmp[2];
			this->cmd.SetRGB.Green = (uint16_t)u32Tmp[3];
			this->cmd.SetRGB.Blue = (uint16_t)u32Tmp[4];
			break;

		case CMD_SETRGBW:
			if(ret != 6)
			{
				return; //Error fetching command
			}
			this->cmd.SetRGBW.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.SetRGBW.Red = (uint16_t)u32Tmp[2];
			this->cmd.SetRGBW.Green = (uint16_t)u32Tmp[3];
			this->cmd.SetRGBW.Blue = (uint16_t)u32Tmp[4];
			this->cmd.SetRGBW.White = (uint16_t)u32Tmp[5];
			break;

		case CMD_SETHSV:
			if(ret != 5)
			{
				return; //Error fetching command
			}
			this->cmd.SetHSV.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.SetHSV.Hue = (uint16_t)u32Tmp[2];
			this->cmd.SetHSV.Saturation = (uint16_t)u32Tmp[3];
			this->cmd.SetHSV.Value = (uint16_t)u32Tmp[4];
			break;

		case CMD_STROBERGB:
			if(ret != 7)
			{
				return; //Error fetching command
			}
			this->cmd.StrobeRGB.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.StrobeRGB.Period = (uint8_t)u32Tmp[2];
			this->cmd.StrobeRGB.Count = (uint8_t)u32Tmp[3];
			this->cmd.StrobeRGB.Red = (uint16_t)u32Tmp[4];
			this->cmd.StrobeRGB.Green = (uint16_t)u32Tmp[5];
			this->cmd.StrobeRGB.Blue = (uint16_t)u32Tmp[6];
			break;

		case CMD_STROBERGBW:
			if(ret != 8)
			{
				return; //Error fetching command
			}
			this->cmd.StrobeRGBW.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.StrobeRGBW.Period = (uint8_t)u32Tmp[2];
			this->cmd.StrobeRGBW.Count = (uint8_t)u32Tmp[3];
			this->cmd.StrobeRGBW.Red = (uint16_t)u32Tmp[4];
			this->cmd.StrobeRGBW.Green = (uint16_t)u32Tmp[5];
			this->cmd.StrobeRGBW.Blue = (uint16_t)u32Tmp[6];
			this->cmd.StrobeRGBW.White = (uint16_t)u32Tmp[7];
			break;

		case CMD_STROBEHSV:
			if(ret != 7)
			{
				return; //Error fetching command
			}
			this->cmd.StrobeHSV.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.StrobeHSV.Period = (uint8_t)u32Tmp[2];
			this->cmd.StrobeHSV.Count = (uint8_t)u32Tmp[3];
			this->cmd.StrobeHSV.Hue = (uint16_t)u32Tmp[4];
			this->cmd.StrobeHSV.Saturation = (uint16_t)u32Tmp[5];
			this->cmd.StrobeHSV.Value = (uint16_t)u32Tmp[6];
			break;

		case CMD_FADEHUE:
			if(ret != 7)
			{
				return; //Error fetching command
			}
			this->cmd.FadeHue.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.FadeHue.Period = (uint16_t)u32Tmp[2];
			this->cmd.FadeHue.Hue_Max = (uint16_t)u32Tmp[3];
			this->cmd.FadeHue.Hue_Min = (uint16_t)u32Tmp[4];
			this->cmd.FadeHue.Saturation = (uint16_t)u32Tmp[5];
			this->cmd.FadeHue.Value = (uint16_t)u32Tmp[6];
			break;

		case CMD_FADESATURATION:
			if(ret != 7)
			{
				return; //Error fetching command
			}
			this->cmd.FadeSaturation.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.FadeSaturation.Period = (uint16_t)u32Tmp[2];
			this->cmd.FadeSaturation.Hue = (uint16_t)u32Tmp[3];
			this->cmd.FadeSaturation.Saturation_Max = (uint16_t)u32Tmp[4];
			this->cmd.FadeSaturation.Saturation_Min = (uint16_t)u32Tmp[5];
			this->cmd.FadeSaturation.Value = (uint16_t)u32Tmp[6];
			break;

		case CMD_FADEVALUE:
			if(ret != 7)
			{
				return; //Error fetching command
			}
			this->cmd.FadeValue.GroupID = (uint8_t)u32Tmp[1];
			this->cmd.FadeValue.Period = (uint16_t)u32Tmp[2];
			this->cmd.FadeValue.Hue = (uint16_t)u32Tmp[3];
			this->cmd.FadeValue.Saturation = (uint16_t)u32Tmp[4];
			this->cmd.FadeValue.Value_Max = (uint16_t)u32Tmp[5];
			this->cmd.FadeValue.Value_Min = (uint16_t)u32Tmp[6];
			break;

		case CMD_GETTEMPERATURE:
			if(ret != 2)
			{
				return; //Error fetching command
			}
			this->cmd.GetTemperature.SlaveAddress = u32Tmp[1];
			break; //Nothing to do

		default:
			return; //No valid command received!
	}

	//A valid command was received and decoded successfully!
	this->cmd_available = true;
}


bool LED_SerialParser::Available(void)
{
	return this->cmd_available;
}


void LED_SerialParser::ReadCMD(NRF24L01_DataPacket *_cmd)
{
	*_cmd = this->cmd; //Copy data
	this->cmd_available = false;
}
