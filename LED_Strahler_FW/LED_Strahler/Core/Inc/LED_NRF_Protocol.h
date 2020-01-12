/*
 * LED_NRF_Protocol.h
 *
 *  Created on: Nov 9, 2019
 *      Author: KaiLi
 */

#ifndef INC_LED_NRF_PROTOCOL_H_
#define INC_LED_NRF_PROTOCOL_H_


//NRF24L01+ settings
#define LED_NRF24L01_CHANNEL			115	//2.515MHz
#define LED_NRF24L01_PAYLOAD			12
#define LED_NRF24L01_DATARATE			NRF24L01_DataRate_1M
#define LED_NRF24L01_OUTPUT_POWER		NRF24L01_OutputPower_0dBm

#define LED_NRF24LO1_BROADCAST_ADDR		0x12345678
#define LED_NRF24L01_BASE_ADDR			0x87654321


// Define commands
#define CMD_PINGREQUEST		0x00
#define CMD_SETGROUP		0x01
#define CMD_SETTIMEBASE		0x02
#define CMD_SETRGB			0x03
#define CMD_SETRGBW			0x04
#define CMD_SETHSV			0x05
#define CMD_STROBERGB		0x06
#define CMD_STROBERGBW		0x07
#define CMD_STROBEHSV		0x08
#define CMD_FADEHUE			0x09
#define CMD_FADESATURATION	0x0A
#define CMD_FADEVALUE		0x0B
#define CMD_GETTEMPERATURE	0x10
#define CMD_IGNORE			0xFF

#define CMD_PINGREQUESTANSWER		0x80
#define CMD_GETTEMPERATUREANSWER	0x90

// Define standard groups
#define BROADCAST_GROUP		0x00

// Define data payloads
struct __attribute__((packed)) NRF24L01_PingRequest
{
    uint8_t CMD;
};

struct __attribute__((packed)) NRF24L01_SetGroup
{
    uint8_t CMD;
    uint32_t SlaveAddress;
    uint8_t GroupID;
};

struct __attribute__((packed)) NRF24L01_SetTimebase
{
    uint8_t CMD;
    uint32_t Timebase;
};

struct __attribute__((packed)) NRF24L01_SetRGB
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Red;
    uint16_t Green;
    uint16_t Blue;
};

struct __attribute__((packed)) NRF24L01_SetRGBW
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Red;
    uint16_t Green;
    uint16_t Blue;
    uint16_t White;
};

struct __attribute__((packed)) NRF24L01_SetHSV
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Hue;
    uint16_t Saturation;
    uint16_t Value;
};

struct __attribute__((packed)) NRF24L01_StrobeRGB
{
    uint8_t CMD;
    uint8_t GroupID;
    uint8_t Period;
    uint8_t Count;
    uint16_t Red;
    uint16_t Green;
    uint16_t Blue;
};

struct __attribute__((packed)) NRF24L01_StrobeRGBW
{
    uint8_t CMD;
    uint8_t GroupID;
    uint8_t Period;
    uint8_t Count;
    uint16_t Red;
    uint16_t Green;
    uint16_t Blue;
    uint16_t White;
};

struct __attribute__((packed)) NRF24L01_StrobeHSV
{
    uint8_t CMD;
    uint8_t GroupID;
    uint8_t Period;
    uint8_t Count;
    uint16_t Hue;
    uint16_t Saturation;
    uint16_t Value;
};

struct __attribute__((packed)) NRF24L01_FadeHue
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Period;
    uint16_t Hue_Max;
    uint16_t Hue_Min;
    uint16_t Saturation;
    uint16_t Value;
};

struct __attribute__((packed)) NRF24L01_FadeSaturation
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Period;
    uint16_t Hue;
    uint16_t Saturation_Max;
    uint16_t Saturation_Min;
    uint16_t Value;
};

struct __attribute__((packed)) NRF24L01_FadeValue
{
    uint8_t CMD;
    uint8_t GroupID;
    uint16_t Period;
    uint16_t Hue;
    uint16_t Saturation;
    uint16_t Value_Max;
    uint16_t Value_Min;
};

struct __attribute__((packed)) NRF24L01_GetTemperature
{
    uint8_t CMD;
    uint32_t SlaveAddress;
};

struct __attribute__((packed)) NRF24L01_PingAnswer
{
	uint8_t CMD;
    uint32_t SlaveAddress;
};

struct __attribute__((packed)) NRF24L01_GetTemperatureAnswer
{
	uint8_t CMD;
    float LED_Temperature;
};

// Union for data packets
typedef union NRF24L01_DataPacket
{
	uint8_t	CMD;
	uint8_t	Data[12];
	struct NRF24L01_PingRequest		PingRequest;
	struct NRF24L01_SetGroup		SetGroup;
	struct NRF24L01_SetTimebase		SetTimebase;
	struct NRF24L01_SetRGB			SetRGB;
	struct NRF24L01_SetRGBW			SetRGBW;
	struct NRF24L01_SetHSV			SetHSV;
	struct NRF24L01_StrobeRGB		StrobeRGB;
	struct NRF24L01_StrobeRGBW		StrobeRGBW;
	struct NRF24L01_StrobeHSV		StrobeHSV;
	struct NRF24L01_FadeHue			FadeHue;
	struct NRF24L01_FadeSaturation	FadeSaturation;
	struct NRF24L01_FadeValue		FadeValue;
	struct NRF24L01_GetTemperature	GetTemperature;
	struct NRF24L01_PingAnswer		PingAnswer;
	struct NRF24L01_GetTemperatureAnswer GetTemperatureAnswer;
}NRF24L01_DataPacket;



#endif /* INC_LED_NRF_PROTOCOL_H_ */
