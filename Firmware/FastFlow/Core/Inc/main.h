/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define ENCODER_A_Pin GPIO_PIN_6
#define ENCODER_A_GPIO_Port GPIOA
#define ENCODER_B_Pin GPIO_PIN_7
#define ENCODER_B_GPIO_Port GPIOA
#define ENCODER_BUTTON_Pin GPIO_PIN_4
#define ENCODER_BUTTON_GPIO_Port GPIOC
#define DISPLAY_D0_Pin GPIO_PIN_0
#define DISPLAY_D0_GPIO_Port GPIOB
#define DISPLAY_D1_Pin GPIO_PIN_1
#define DISPLAY_D1_GPIO_Port GPIOB
#define DISPLAY_D2_Pin GPIO_PIN_2
#define DISPLAY_D2_GPIO_Port GPIOB
#define DISPLAY_D3_Pin GPIO_PIN_10
#define DISPLAY_D3_GPIO_Port GPIOB
#define DISPLAY_D4_Pin GPIO_PIN_12
#define DISPLAY_D4_GPIO_Port GPIOB
#define DISPLAY_D5_Pin GPIO_PIN_13
#define DISPLAY_D5_GPIO_Port GPIOB
#define DISPLAY_D6_Pin GPIO_PIN_14
#define DISPLAY_D6_GPIO_Port GPIOB
#define DISPLAY_D7_Pin GPIO_PIN_15
#define DISPLAY_D7_GPIO_Port GPIOB
#define DISPLAY_READ_WRITE_Pin GPIO_PIN_6
#define DISPLAY_READ_WRITE_GPIO_Port GPIOC
#define DISPLAY_REGISTER_SELECT_Pin GPIO_PIN_7
#define DISPLAY_REGISTER_SELECT_GPIO_Port GPIOC
#define DISPLAY_ENABLE_Pin GPIO_PIN_8
#define DISPLAY_ENABLE_GPIO_Port GPIOC
#define DOOR_CLOSED_Pin GPIO_PIN_9
#define DOOR_CLOSED_GPIO_Port GPIOC
#define HEATER_PWM_Pin GPIO_PIN_8
#define HEATER_PWM_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SPI3_CS_MAX31865_Pin GPIO_PIN_15
#define SPI3_CS_MAX31865_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BUZZER_PWM_Pin GPIO_PIN_6
#define BUZZER_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
