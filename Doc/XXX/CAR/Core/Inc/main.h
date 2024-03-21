/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define M2IN4_Pin GPIO_PIN_1
#define M2IN4_GPIO_Port GPIOA
#define M2IN3_Pin GPIO_PIN_2
#define M2IN3_GPIO_Port GPIOA
#define M2IN2_Pin GPIO_PIN_3
#define M2IN2_GPIO_Port GPIOA
#define M2IN1_Pin GPIO_PIN_4
#define M2IN1_GPIO_Port GPIOA
#define M1IN4_Pin GPIO_PIN_5
#define M1IN4_GPIO_Port GPIOB
#define M1IN3_Pin GPIO_PIN_6
#define M1IN3_GPIO_Port GPIOB
#define M1IN2_Pin GPIO_PIN_7
#define M1IN2_GPIO_Port GPIOB
#define M1IN1_Pin GPIO_PIN_8
#define M1IN1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define M1IN1_0 HAL_GPIO_WritePin(M1IN1_GPIO_Port,M1IN1_Pin,GPIO_PIN_RESET)
#define M1IN1_1 HAL_GPIO_WritePin(M1IN1_GPIO_Port,M1IN1_Pin,GPIO_PIN_SET)
#define M1IN2_0 HAL_GPIO_WritePin(M1IN2_GPIO_Port,M1IN2_Pin,GPIO_PIN_RESET)
#define M1IN2_1 HAL_GPIO_WritePin(M1IN2_GPIO_Port,M1IN2_Pin,GPIO_PIN_SET)
#define M1IN3_0 HAL_GPIO_WritePin(M1IN3_GPIO_Port,M1IN3_Pin,GPIO_PIN_RESET)
#define M1IN3_1 HAL_GPIO_WritePin(M1IN3_GPIO_Port,M1IN3_Pin,GPIO_PIN_SET)
#define M1IN4_0 HAL_GPIO_WritePin(M1IN4_GPIO_Port,M1IN4_Pin,GPIO_PIN_RESET)
#define M1IN4_1 HAL_GPIO_WritePin(M1IN4_GPIO_Port,M1IN4_Pin,GPIO_PIN_SET)

#define M2IN1_0 HAL_GPIO_WritePin(M2IN1_GPIO_Port,M2IN1_Pin,GPIO_PIN_RESET)
#define M2IN1_1 HAL_GPIO_WritePin(M2IN1_GPIO_Port,M2IN1_Pin,GPIO_PIN_SET)
#define M2IN2_0 HAL_GPIO_WritePin(M2IN2_GPIO_Port,M2IN2_Pin,GPIO_PIN_RESET)
#define M2IN2_1 HAL_GPIO_WritePin(M2IN2_GPIO_Port,M2IN2_Pin,GPIO_PIN_SET)
#define M2IN3_0 HAL_GPIO_WritePin(M2IN3_GPIO_Port,M2IN3_Pin,GPIO_PIN_RESET)
#define M2IN3_1 HAL_GPIO_WritePin(M2IN3_GPIO_Port,M2IN3_Pin,GPIO_PIN_SET)
#define M2IN4_0 HAL_GPIO_WritePin(M2IN4_GPIO_Port,M2IN4_Pin,GPIO_PIN_RESET)
#define M2IN4_1 HAL_GPIO_WritePin(M2IN4_GPIO_Port,M2IN4_Pin,GPIO_PIN_SET)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
