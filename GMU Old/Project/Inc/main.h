/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
void systick_callback(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NEUTRAL_Pin GPIO_PIN_0
#define NEUTRAL_GPIO_Port GPIOA
#define AIR_PRESS_Pin GPIO_PIN_1
#define AIR_PRESS_GPIO_Port GPIOA
#define GEAR_CUT_Pin GPIO_PIN_4
#define GEAR_CUT_GPIO_Port GPIOA
#define CLUTCH_POS_Pin GPIO_PIN_5
#define CLUTCH_POS_GPIO_Port GPIOA
#define GEAR_POS_Pin GPIO_PIN_7
#define GEAR_POS_GPIO_Port GPIOA
#define CLUTCH_Pin GPIO_PIN_12
#define CLUTCH_GPIO_Port GPIOB
#define GEAR_DOWN_Pin GPIO_PIN_14
#define GEAR_DOWN_GPIO_Port GPIOB
#define GEAR_UP_Pin GPIO_PIN_8
#define GEAR_UP_GPIO_Port GPIOA
#define STATUS_LED_Pin GPIO_PIN_7
#define STATUS_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/*
#define GEAR_DOWN_Pin GPIO_PIN_14
#define GEAR_DOWN_GPIO_Port GPIOB
#define GEAR_UP_Pin GPIO_PIN_8
#define GEAR_UP_GPIO_Port GPIOA
*/
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
