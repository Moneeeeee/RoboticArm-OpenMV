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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "retarget.h"
#include "string.h"
#include "Motor.h"
#include "Control.h"



// PID 控制器参�???
#define KP 2   // 比例系数
#define KI 1   // 积分系数
#define KD 1   // 微分系数

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define BUFFER_SIZE 64  // 定义接收缓冲区大小为 64 字节
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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SERVO_1_Pin GPIO_PIN_0
#define SERVO_1_GPIO_Port GPIOA
#define SERVO_2_Pin GPIO_PIN_1
#define SERVO_2_GPIO_Port GPIOA
#define Trak_L_Pin GPIO_PIN_2
#define Trak_L_GPIO_Port GPIOA
#define Trak_R_Pin GPIO_PIN_3
#define Trak_R_GPIO_Port GPIOA
#define SERVO_3_Pin GPIO_PIN_6
#define SERVO_3_GPIO_Port GPIOA
#define SERVO_4_Pin GPIO_PIN_7
#define SERVO_4_GPIO_Port GPIOA
#define MOTO1_Pin GPIO_PIN_0
#define MOTO1_GPIO_Port GPIOB
#define MOTO2_Pin GPIO_PIN_1
#define MOTO2_GPIO_Port GPIOB
#define MOTO3_Pin GPIO_PIN_10
#define MOTO3_GPIO_Port GPIOB
#define MOTO4_Pin GPIO_PIN_11
#define MOTO4_GPIO_Port GPIOB
#define DIN2_Pin GPIO_PIN_15
#define DIN2_GPIO_Port GPIOA
#define DIN1_Pin GPIO_PIN_3
#define DIN1_GPIO_Port GPIOB
#define CIN2_Pin GPIO_PIN_4
#define CIN2_GPIO_Port GPIOB
#define CIN1_Pin GPIO_PIN_5
#define CIN1_GPIO_Port GPIOB
#define BIN2_Pin GPIO_PIN_6
#define BIN2_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_7
#define BIN1_GPIO_Port GPIOB
#define AIN2_Pin GPIO_PIN_8
#define AIN2_GPIO_Port GPIOB
#define AIN1_Pin GPIO_PIN_9
#define AIN1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
