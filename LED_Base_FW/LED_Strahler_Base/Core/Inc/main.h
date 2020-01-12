/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_MODE_Pin GPIO_PIN_13
#define SW_MODE_GPIO_Port GPIOC
#define BTN_PING_Pin GPIO_PIN_15
#define BTN_PING_GPIO_Port GPIOC
#define BT_EN_Pin GPIO_PIN_0
#define BT_EN_GPIO_Port GPIOC
#define BT_STA_Pin GPIO_PIN_1
#define BT_STA_GPIO_Port GPIOC
#define BT_TX_Pin GPIO_PIN_2
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_3
#define BT_RX_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOA
#define OP_OUT_Pin GPIO_PIN_6
#define OP_OUT_GPIO_Port GPIOA
#define OP_INP_Pin GPIO_PIN_7
#define OP_INP_GPIO_Port GPIOA
#define NRF_IRQ_Pin GPIO_PIN_4
#define NRF_IRQ_GPIO_Port GPIOC
#define NRF_IRQ_EXTI_IRQn EXTI4_IRQn
#define OP_INM_Pin GPIO_PIN_5
#define OP_INM_GPIO_Port GPIOC
#define NRF_CS_Pin GPIO_PIN_1
#define NRF_CS_GPIO_Port GPIOB
#define NRF_CE_Pin GPIO_PIN_2
#define NRF_CE_GPIO_Port GPIOB
#define NRF_CLK_Pin GPIO_PIN_13
#define NRF_CLK_GPIO_Port GPIOB
#define NRF_MISO_Pin GPIO_PIN_14
#define NRF_MISO_GPIO_Port GPIOB
#define NRF_MOSI_Pin GPIO_PIN_15
#define NRF_MOSI_GPIO_Port GPIOB
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWDCLK_Pin GPIO_PIN_14
#define SWDCLK_GPIO_Port GPIOA
#define PC_TX_Pin GPIO_PIN_10
#define PC_TX_GPIO_Port GPIOC
#define PC_RX_Pin GPIO_PIN_11
#define PC_RX_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
