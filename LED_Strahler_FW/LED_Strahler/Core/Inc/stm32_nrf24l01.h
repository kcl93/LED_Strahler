/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.net
 * @link    http://stm32f4-discovery.net/2015/09/hal-library-25-nrf24l01-for-stm32fxxx/
 * @version v1.0
 * @ide     Keil uVision
 * @license MIT
 * @brief   Library template 
 *	
\verbatim
   ----------------------------------------------------------------------
    Copyright (c) 2016 Tilen Majerle

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software, 
    and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
    AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
   ----------------------------------------------------------------------
\endverbatim
 */
#ifndef NRF24L01_H
#define NRF24L01_H 100

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup STM32Fxxx_HAL_Libraries
 * @{
 */

/**
 * @defgroup NRF24L01P
 * @brief    nRF24L01+ library for STM32xxx devices - http://stm32f4-discovery.net/2015/09/hal-library-25-nrf24l01-for-stm32fxxx/
 * @{
 *
 * This library allows you to work with nRF24L01+ modules.
 * 
 * You can send and receive data from nRF24L01+ modules.
 * 
 * \par Default pinout
 * 	
\verbatim
NRF24L01+	STM32Fxxx	DESCRIPTION

GND			GND			Ground
VCC			3.3V		3.3V
CE			PD8			RF activated pin
CSN			PD7			Chip select pin for SPI
SCK			PC10		SCK pin for SPI
MOSI		PC12		MOSI pin for SPI
MISO		PC11		MISO pin for SPI
IRQ			Not used	Interrupt pin. Goes low when active. Pin functionality is active, but not used in library
\endverbatim 	
 *
 * IRQ pin is not used in this library, but its functionality is enabled by this software.
 *
 * You can still set any pin on Fxxx to be an external interrupt and handle interrupts from nRF24L01+ module.
 *
 * The easiest way to that is to use @ref EXTI library and attach interrupt functionality to this pin
 * 
 * \par Custom pinout
 *
 * Add lines below in your defines.h file if you want to change default pinout:
 *
\code
//Change SPI used. Refer to TM SPI library to check which pins are for SPI
#define NRF24L01_SPI				SPI3
#define NRF24L01_SPI_PINS			SPI_PinsPack_2

//Change CSN pin. This is for SPI communication
#define NRF24L01_CSN_PORT			GPIOD
#define NRF24L01_CSN_PIN			GPIO_Pin_7

//Change CE pin. This pin is used to enable/disable transmitter/receiver functionality
#define NRF24L01_CE_PORT			GPIOD
#define NRF24L01_CE_PIN				GPIO_Pin_8
\endcode
 *
 * \par Changelog
 *
\verbatim
 Version 1.0
  - First release
\endverbatim
 *
 * \par Dependencies
 *
\verbatim
 - STM32Fxxx HAL
 - defines.h
 - TM SPI
 - TM GPIO
\endverbatim
 */
#include "stm32f0xx_hal.h"
#include "stm32_nrf24l01_hal.h"
#include "main.h"


/* NRF24L01+ registers*/
#define NRF24L01_REG_CONFIG			0x00	//Configuration Register
#define NRF24L01_REG_EN_AA			0x01	//Enable �Auto Acknowledgment� Function
#define NRF24L01_REG_EN_RXADDR		0x02	//Enabled RX Addresses
#define NRF24L01_REG_SETUP_AW		0x03	//Setup of Address Widths (common for all data pipes)
#define NRF24L01_REG_SETUP_RETR		0x04	//Setup of Automatic Retransmission
#define NRF24L01_REG_RF_CH			0x05	//RF Channel
#define NRF24L01_REG_RF_SETUP		0x06	//RF Setup Register
#define NRF24L01_REG_STATUS			0x07	//Status Register
#define NRF24L01_REG_OBSERVE_TX		0x08	//Transmit observe registerf
#define NRF24L01_REG_RPD			0x09
#define NRF24L01_REG_RX_ADDR_P0		0x0A	//Receive address data pipe 0. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P1		0x0B	//Receive address data pipe 1. 5 Bytes maximum length.
#define NRF24L01_REG_RX_ADDR_P2		0x0C	//Receive address data pipe 2. Only LSB
#define NRF24L01_REG_RX_ADDR_P3		0x0D	//Receive address data pipe 3. Only LSB
#define NRF24L01_REG_RX_ADDR_P4		0x0E	//Receive address data pipe 4. Only LSB
#define NRF24L01_REG_RX_ADDR_P5		0x0F	//Receive address data pipe 5. Only LSB
#define NRF24L01_REG_TX_ADDR		0x10	//Transmit address. Used for a PTX device only
#define NRF24L01_REG_RX_PW_P0		0x11
#define NRF24L01_REG_RX_PW_P1		0x12
#define NRF24L01_REG_RX_PW_P2		0x13
#define NRF24L01_REG_RX_PW_P3		0x14
#define NRF24L01_REG_RX_PW_P4		0x15
#define NRF24L01_REG_RX_PW_P5		0x16
#define NRF24L01_REG_FIFO_STATUS	0x17	//FIFO Status Register
#define NRF24L01_REG_DYNPD			0x1C	//Enable dynamic payload length
#define NRF24L01_REG_FEATURE		0x1D

/* Registers default values */
#define NRF24L01_REG_DEFAULT_VAL_CONFIG			0x08
#define NRF24L01_REG_DEFAULT_VAL_EN_AA			0x3F
#define NRF24L01_REG_DEFAULT_VAL_EN_RXADDR		0x03
#define NRF24L01_REG_DEFAULT_VAL_SETUP_AW		0x03
#define NRF24L01_REG_DEFAULT_VAL_SETUP_RETR		0x03
#define NRF24L01_REG_DEFAULT_VAL_RF_CH			0x02
#define NRF24L01_REG_DEFAULT_VAL_RF_SETUP		0x0E
#define NRF24L01_REG_DEFAULT_VAL_STATUS			0x0E
#define NRF24L01_REG_DEFAULT_VAL_OBSERVE_TX		0x00
#define NRF24L01_REG_DEFAULT_VAL_RPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_0	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_1	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_2	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_3	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P0_4	0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_0	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_1	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_2	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_3	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P1_4	0xC2
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P2		0xC3
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P3		0xC4
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P4		0xC5
#define NRF24L01_REG_DEFAULT_VAL_RX_ADDR_P5		0xC6
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_0		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_1		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_2		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_3		0xE7
#define NRF24L01_REG_DEFAULT_VAL_TX_ADDR_4		0xE7
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P0		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P1		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P2		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P3		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P4		0x00
#define NRF24L01_REG_DEFAULT_VAL_RX_PW_P5		0x00
#define NRF24L01_REG_DEFAULT_VAL_FIFO_STATUS	0x11
#define NRF24L01_REG_DEFAULT_VAL_DYNPD			0x00
#define NRF24L01_REG_DEFAULT_VAL_FEATURE		0x00

/* Configuration register*/
#define NRF24L01_MASK_RX_DR		6
#define NRF24L01_MASK_TX_DS		5
#define NRF24L01_MASK_MAX_RT	4
#define NRF24L01_EN_CRC			3
#define NRF24L01_CRCO			2
#define NRF24L01_PWR_UP			1
#define NRF24L01_PRIM_RX		0

/* Enable auto acknowledgment*/
#define NRF24L01_ENAA_P5		5
#define NRF24L01_ENAA_P4		4
#define NRF24L01_ENAA_P3		3
#define NRF24L01_ENAA_P2		2
#define NRF24L01_ENAA_P1		1
#define NRF24L01_ENAA_P0		0

/* Enable rx addresses */
#define NRF24L01_ERX_P5			5
#define NRF24L01_ERX_P4			4
#define NRF24L01_ERX_P3			3
#define NRF24L01_ERX_P2			2
#define NRF24L01_ERX_P1			1
#define NRF24L01_ERX_P0			0

/* Setup of address width */
#define NRF24L01_AW				0 //2 bits

/* Setup of auto re-transmission*/
#define NRF24L01_ARD			4 //4 bits
#define NRF24L01_ARC			0 //4 bits

/* RF setup register*/
#define NRF24L01_PLL_LOCK		4
#define NRF24L01_RF_DR_LOW		5
#define NRF24L01_RF_DR_HIGH		3
#define NRF24L01_RF_DR			3
#define NRF24L01_RF_PWR			1 //2 bits

/* General status register */
#define NRF24L01_RX_DR			6
#define NRF24L01_TX_DS			5
#define NRF24L01_MAX_RT			4
#define NRF24L01_RX_P_NO		1 //3 bits
#define NRF24L01_TX_FULL		0

/* Transmit observe register */
#define NRF24L01_PLOS_CNT		4 //4 bits
#define NRF24L01_ARC_CNT		0 //4 bits

/* FIFO status*/
#define NRF24L01_TX_REUSE		6
#define NRF24L01_FIFO_FULL		5
#define NRF24L01_TX_EMPTY		4
#define NRF24L01_RX_FULL		1
#define NRF24L01_RX_EMPTY		0

//Dynamic length
#define NRF24L01_DPL_P0			0
#define NRF24L01_DPL_P1			1
#define NRF24L01_DPL_P2			2
#define NRF24L01_DPL_P3			3
#define NRF24L01_DPL_P4			4
#define NRF24L01_DPL_P5			5

/* Transmitter power*/
#define NRF24L01_M18DBM			0 //-18 dBm
#define NRF24L01_M12DBM			1 //-12 dBm
#define NRF24L01_M6DBM			2 //-6 dBm
#define NRF24L01_0DBM			3 //0 dBm

/* Data rates */
#define NRF24L01_2MBPS			0
#define NRF24L01_1MBPS			1
#define NRF24L01_250KBPS		2

/* Configuration, 2 bytes CRC, disable all interrupts on IRQ pin */
#define NRF24L01_CONFIG			((1 << NRF24L01_EN_CRC) | (1 << NRF24L01_CRCO))

/* Instruction Mnemonics */
#define NRF24L01_REGISTER_MASK				0x1F

#define NRF24L01_READ_REGISTER_MASK(reg)	(0x00 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_WRITE_REGISTER_MASK(reg)	(0x20 | (NRF24L01_REGISTER_MASK & reg)) //Last 5 bits will indicate reg. address
#define NRF24L01_R_RX_PAYLOAD_MASK			0x61
#define NRF24L01_W_TX_PAYLOAD_MASK			0xA0
#define NRF24L01_FLUSH_TX_MASK				0xE1
#define NRF24L01_FLUSH_RX_MASK				0xE2
#define NRF24L01_REUSE_TX_PL_MASK			0xE3
#define NRF24L01_ACTIVATE_MASK				0x50
#define NRF24L01_R_RX_PL_WID_MASK			0x60
#define NRF24L01_W_ACK_PAYLOAD_MASK(reg)	(0xA8 | (0x07 & reg))
#define NRF24L01_W_TX_PAYLOAD_NOACK_MASK	0xB0
#define NRF24L01_NOP_MASK					0xFF

/* Flush FIFOs */
#define NRF24L01_FLUSH_TX					do { NRF24L01_CSN_LOW; NRF24L01_SPI_Send(NRF24L01_FLUSH_TX_MASK); NRF24L01_CSN_HIGH; } while (0)
#define NRF24L01_FLUSH_RX					do { NRF24L01_CSN_LOW; NRF24L01_SPI_Send(NRF24L01_FLUSH_RX_MASK); NRF24L01_CSN_HIGH; } while (0)

#define NRF24L01_TRANSMISSON_OK 			0
#define NRF24L01_MESSAGE_LOST   			1

#define NRF24L01_CHECK_BIT(reg, bit)       (reg & (1 << bit))


/**
 * @defgroup NRF24L01P_Macros
 * @brief    Library defines
 * @{
 */

/* Interrupt masks */
#define NRF24L01_IRQ_DATA_READY     0x40 /*!< Data ready for receive */
#define NRF24L01_IRQ_TRAN_OK        0x20 /*!< Transmission went OK */
#define NRF24L01_IRQ_MAX_RT         0x10 /*!< Max retransmissions reached, last transmission failed */

/**
 * @}
 */
 
/**
 * @defgroup NRF24L01P_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  Interrupt structure 
 */
typedef union _NRF24L01_IRQ_t {
	struct {
		uint8_t reserved0:4;
		uint8_t MaxRT:1;     /*!< Set to 1 if MAX retransmissions flag is set */
		uint8_t DataSent:1;  /*!< Set to 1 if last transmission is OK */
		uint8_t DataReady:1; /*!< Set to 1 if data are ready to be read */
		uint8_t reserved1:1;
	} F;
	uint8_t Status;          /*!< NRF status register value */
} NRF24L01_IRQ_t;

/**
 * @brief  Transmission status enumeration
 */
typedef enum _NRF24L01_Transmit_Status_t {
	NRF24L01_Transmit_Status_Lost = 0x00,   /*!< Message is lost, reached maximum number of retransmissions */
	NRF24L01_Transmit_Status_Ok = 0x01,     /*!< Message sent successfully */
	NRF24L01_Transmit_Status_Sending = 0xFF /*!< Message is still sending */
} NRF24L01_Transmit_Status_t;

/**
 * @brief  Data rate enumeration
 */
typedef enum _NRF24L01_DataRate_t {
	NRF24L01_DataRate_2M = 0x00, /*!< Data rate set to 2Mbps */
	NRF24L01_DataRate_1M,        /*!< Data rate set to 1Mbps */
	NRF24L01_DataRate_250k       /*!< Data rate set to 250kbps */
} NRF24L01_DataRate_t;

/**
 * @brief  Output power enumeration
 */
typedef enum _NRF24L01_OutputPower_t {
	NRF24L01_OutputPower_M18dBm = 0x00,/*!< Output power set to -18dBm */
	NRF24L01_OutputPower_M12dBm,       /*!< Output power set to -12dBm */
	NRF24L01_OutputPower_M6dBm,        /*!< Output power set to -6dBm */
	NRF24L01_OutputPower_0dBm          /*!< Output power set to 0dBm */
} NRF24L01_OutputPower_t;

/**
 * @}
 */

/**
 * @defgroup NRF24L01P_Functions
 * @brief    Library Functions
 *
 * Here are listed very basic functions to work with NRF modules
 *
 * @{
 */

/**
 * @brief  Initializes NRF24L01+ module
 * @param  channel: channel you will use for communication, from 0 to 125 eg. working frequency from 2.4 to 2.525 GHz
 * @param  payload_size: maximum data to be sent in one packet from one NRF to another.
 * @note   Maximal payload size is 32bytes
 * @retval 1
 */
uint8_t NRF24L01_Init(uint8_t channel, uint8_t payload_size);

/**
 * @brief  Sets own address. This is used for settings own id when communication with other modules
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetMyAddress(uint8_t* adr);

/**
 * @brief  Sets address you will communicate with
 * @note   "Own" address of one device must be the same as "TX" address of other device (and vice versa),
 *         if you want to get successful communication
 * @param  *adr: Pointer to 5-bytes length array with address
 * @retval None
 */
void NRF24L01_SetTxAddress(uint8_t* adr);

/**
 * @brief  Gets number of retransmissions needed in last transmission
 * @param  None
 * @retval Number of retransmissions, between 0 and 15.
 */
uint8_t NRF24L01_GetRetransmissionsCount(void);

/**
 * @brief  Sets NRF24L01+ to TX mode
 * @note   In this mode is NRF able to send data to another NRF module
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpTx(void);

/**
 * @brief  Sets NRF24L01+ to RX mode
 * @note   In this mode is NRF able to receive data from another NRF module.
 *         This is default mode and should be used all the time, except when sending data
 * @param  None
 * @retval None
 */
void NRF24L01_PowerUpRx(void);

/**
 * @brief  Sets NRF24L01+ to power down mode
 * @note   In power down mode, you are not able to transmit/receive data.
 *         You can wake up device using @ref NRF24L01_PowerUpTx() or @ref NRF24L01_PowerUpRx() functions
 * @param  None
 * @retval None
 */
void NRF24L01_PowerDown(void);

/**
 * @brief  Gets transmissions status
 * @param  None
 * @retval Transmission status. Return is based on @ref NRF24L01_Transmit_Status_t enumeration
 */
NRF24L01_Transmit_Status_t NRF24L01_GetTransmissionStatus(void);

/**
 * @brief  Transmits data with NRF24L01+ to another NRF module
 * @param  *data: Pointer to 8-bit array with data.
 *         Maximum length of array can be the same as "payload_size" parameter on initialization
 * @retval None
 */
void NRF24L01_Transmit(uint8_t *data);

/**
 * @brief  Checks if data is ready to be read from NRF24L01+
 * @param  None
 * @retval Data ready status:
 *            - 0: No data available for receive in bufferReturns
 *            - > 0: Data is ready to be collected
 */
uint8_t NRF24L01_DataReady(void);

/**
 * @brief  Gets data from NRF24L01+
 * @param  *data: Pointer to 8-bits array where data from NRF will be saved
 * @retval None
 */
void NRF24L01_GetData(uint8_t *data);

/**
 * @brief  Sets working channel
 * @note   Channel value is just an offset in units MHz from 2.4GHz
 *         For example, if you select channel 65, then operation frequency will be set to 2.465GHz.
 * @param  channel: RF channel where device will operate
 * @retval None 
 */
void NRF24L01_SetChannel(uint8_t channel);

/**
 * @brief  Sets RF parameters for NRF24L01+
 * @param  DataRate: Data rate selection for NRF module. This parameter can be a value of @ref NRF24L01_DataRate_t enumeration
 * @param  OutPwr: Output power selection for NRF module. This parameter can be a value of @ref NRF24L01_OutputPower_t enumeration
 * @retval None
 */
void NRF24L01_SetRF(NRF24L01_DataRate_t DataRate, NRF24L01_OutputPower_t OutPwr);

/**
 * @brief  Gets NRLF+ status register value
 * @param  None
 * @retval Status register from NRF
 */
uint8_t NRF24L01_GetStatus(void);

/**
 * @brief  Reads interrupts from NRF 
 * @param  *IRQ: Pointer to @ref NRF24L01_IRQ_t where IRQ status will be saved
 * @retval IRQ status
 *            - 0: No interrupts are active
 *            - > 0: At least one interrupt is active
 */
uint8_t NRF24L01_Read_Interrupts(NRF24L01_IRQ_t* IRQ);

/**
 * @brief  Clears interrupt status
 * @param  None
 * @retval None
 */
uint8_t NRF24L01_Clear_Interrupts(void);

/* Private */
uint8_t NRF24L01_WriteRegister(uint8_t reg, uint8_t value);
uint8_t NRF24L01_ReadRegister(uint8_t reg);
void NRF24L01_ReadRegisterMulti(uint8_t reg, uint8_t* data, uint8_t count);
void NRF24L01_WriteRegisterMulti(uint8_t reg, uint8_t *data, uint8_t count);
/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif

