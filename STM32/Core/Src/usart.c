/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#define OPENMV_Uart_RX_LENGTH_MAX 9
static uint8_t OpenMV_Uart_Rx_Index = 0;                        // UART接收计数索引
static uint8_t OpenMV_Uart_Rx_Temp  = 0;                         // UART中断接收缓存
uint8_t OpenMV_Uart_Rx_Buffer[OPENMV_Uart_RX_LENGTH_MAX] = {0}; // UART接收缓存
/*********   应用�????   *********/
short OpenMV_Rx_Data[OPENMV_Uart_RX_LENGTH_MAX-3] = {0};             // 接收OpenMV数据
uint8_t OpenMV_Rx_Data_Analysis_State = 0; // OpenMV数据解析状�??
int count_flag = 0;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void OpenMV_Check_Data_Task(void)
{
    static int flag;
    flag = 0;
    if (!OpenMV_Uart_Rx_Index) // 没有数据 �??�??
    {
        OpenMV_Uart_Rx_Index = 0;
        return;
    }


    // 接收长度是否达到
    if (OpenMV_Uart_Rx_Index < 9) // 按需修改 �??7：两个short�?? 5：一个short；依此类推）
    {
        OpenMV_Uart_Rx_Index = 0;
        return;
    }

    // �??查格式是否正�??
    if ((OpenMV_Uart_Rx_Buffer[0] != 0x5b) && (OpenMV_Uart_Rx_Buffer[1] != 0x38) &&// 帧头(�??个字�??)
        (OpenMV_Uart_Rx_Buffer[OpenMV_Uart_Rx_Index - 1] != 0xb5))                  // 帧尾
    {
        HAL_Delay(100);
        // 清零接收缓冲 为下�??次接收准�??
        OpenMV_Uart_Rx_Index = 0;
        return;
    }
        for(int i=0;i<OPENMV_Uart_RX_LENGTH_MAX-3;i++)
        {

    //        OpenMV_Rx_Data[i] = OpenMV_Uart_Rx_Buffer[i+2];
            OpenMV_Rx_Data[i] = OpenMV_Uart_Rx_Buffer[2 + (i << 1) + 1] << 8; // �??8�??
//            printf("%d\r\n",OpenMV_Rx_Data[i]);
            OpenMV_Rx_Data[i] |= OpenMV_Uart_Rx_Buffer[2 + (i << 1) + 0];     // �??8�??

        }

    OpenMV_Rx_Data_Analysis_State = 1; // 解析状�?�置1
    OpenMV_Uart_Rx_Index = 0;
}

extern int chasu;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    if (huart == &huart1)//无线串口
    {
        // 将接收到的数据存入缓冲区
        if (OpenMV_Uart_Rx_Index < OPENMV_Uart_RX_LENGTH_MAX) // 接收缓冲是否溢出
        {
            OpenMV_Uart_Rx_Buffer[OpenMV_Uart_Rx_Index] = huart->Instance->DR;; // 放入缓存
            OpenMV_Uart_Rx_Index++;                                            // 索引�???1
        }
        HAL_UART_Receive_IT(&huart1, OpenMV_Uart_Rx_Buffer + OpenMV_Uart_Rx_Index, 1); // 继续接收下一个字�???

    }
//    if (huart == &huart1)//OPENMV
//    {
//        // 将接收到的数据存入缓冲区
//        if (OpenMV_Uart_Rx_Index2 < OPENMV_Uart_RX_LENGTH_MAX2) // 接收缓冲是否溢出
//        {HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
////            char txData[] = "Hello, fuck!\r\n";
////            HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
//            OpenMV_Uart_Rx_Buffer2[OpenMV_Uart_Rx_Index2] = huart->Instance->DR;; // 放入缓存
//            chasu = (int)OpenMV_Uart_Rx_Buffer2[OpenMV_Uart_Rx_Index2];
//            OpenMV_Uart_Rx_Index2++;                                            // 索引�???1
////            printf("%d\r\n",OpenMV_Uart_Rx_Index2);
//        }
//        HAL_UART_Receive_IT(&huart1, OpenMV_Uart_Rx_Buffer2 + OpenMV_Uart_Rx_Index2, 1); // 继续接收下一个字�???
//
//    }
//    else if (huart == &huart1) {
//        // 假设您的串口接收缓冲区为rx_buffer，缓冲区大小为buffer_size
//        static uint8_t rx_buffer[BUFFER_SIZE];
//        static uint32_t rx_index = 0;
//
//        // 将接收到的数据存入缓冲区
//        rx_buffer[rx_index++] = huart->Instance->DR;
//
////        // 判断是否接收到完整的
////        if (rx_index >= 3 && rx_buffer[rx_index - 1] == '\n' && rx_buffer[rx_index - 2] == '\r')
////        {
//        // 解析接收到的数据
//
////            sscanf((char*)rx_buffer, "%d,%d", &offset_x, &offset_y);
//
//        memset(rx_buffer, 0, BUFFER_SIZE);
//        rx_index = 0;
////        }
//
//        HAL_UART_Receive_IT(huart, &rx_buffer[rx_index], 1);
//    }
}
/* USER CODE END 1 */
