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
#include "stm32f0xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOF
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOF
#define TEMP_Pin GPIO_PIN_1
#define TEMP_GPIO_Port GPIOA
#define DMX_TX_Pin GPIO_PIN_2
#define DMX_TX_GPIO_Port GPIOA
#define DMX_RX_Pin GPIO_PIN_3
#define DMX_RX_GPIO_Port GPIOA
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOA
#define NRF_SCK_Pin GPIO_PIN_5
#define NRF_SCK_GPIO_Port GPIOA
#define NRF_MISO_Pin GPIO_PIN_6
#define NRF_MISO_GPIO_Port GPIOA
#define NRF_MOSI_Pin GPIO_PIN_7
#define NRF_MOSI_GPIO_Port GPIOA
#define LED_R_SLOW_Pin GPIO_PIN_0
#define LED_R_SLOW_GPIO_Port GPIOB
#define LED_G_SLOW_Pin GPIO_PIN_1
#define LED_G_SLOW_GPIO_Port GPIOB
#define NRF_IRQ_Pin GPIO_PIN_2
#define NRF_IRQ_GPIO_Port GPIOB
#define NRF_IRQ_EXTI_IRQn EXTI2_3_IRQn
#define NRF_CE_Pin GPIO_PIN_10
#define NRF_CE_GPIO_Port GPIOB
#define SW_1_Pin GPIO_PIN_12
#define SW_1_GPIO_Port GPIOB
#define SW_2_Pin GPIO_PIN_13
#define SW_2_GPIO_Port GPIOB
#define LED_R_FAST_Pin GPIO_PIN_8
#define LED_R_FAST_GPIO_Port GPIOA
#define LED_G_FAST_Pin GPIO_PIN_9
#define LED_G_FAST_GPIO_Port GPIOA
#define LED_B_FAST_Pin GPIO_PIN_10
#define LED_B_FAST_GPIO_Port GPIOA
#define LED_W_FAST_Pin GPIO_PIN_11
#define LED_W_FAST_GPIO_Port GPIOA
#define LED_B_SLOW_Pin GPIO_PIN_4
#define LED_B_SLOW_GPIO_Port GPIOB
#define LED_W_SLOW_Pin GPIO_PIN_5
#define LED_W_SLOW_GPIO_Port GPIOB
#define UART_TX_Pin GPIO_PIN_6
#define UART_TX_GPIO_Port GPIOB
#define UART_RX_Pin GPIO_PIN_7
#define UART_RX_GPIO_Port GPIOB
#define FAN_PWM_Pin GPIO_PIN_9
#define FAN_PWM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
