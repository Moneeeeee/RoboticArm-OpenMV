/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ChangeTimerPrescaler(TIM_HandleTypeDef *htim, uint32_t newPrescaler) {
    // 停止定时�??
    HAL_TIM_Base_Stop(htim);

    // 修改预分频系�??
    htim->Init.Prescaler = newPrescaler;

    // 重新初始化定时器
    HAL_TIM_Base_Init(htim);

    // 重新启动定时�??
    HAL_TIM_Base_Start(htim);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int offset_x = 0;
int offset_y = 0;
extern uint8_t OpenMV_Rx_Data_Analysis_State;
extern short OpenMV_Rx_Data[6];             // 接收OpenMV数据
extern uint8_t OpenMV_Rx_Data_Analysis_State2;
extern short OpenMV_Rx_Data2[6];             // 接收OpenMV数据
int duoji_1=150;
int duoji_2=180;
int duoji_3=230;
int xunji_status=1;
int setspeed=0;
int chasu=0;
int chasu_flag=1;
int stop_flag=1;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  RetargetInit(&huart1);

  uint8_t rx_buffer[BUFFER_SIZE];  // 声明接收缓冲�?????????????
    uint8_t rx_buffer2[BUFFER_SIZE];  // 声明接收缓冲�?????????????
  uint32_t rx_index = 0;  // 声明接收缓冲区索�?????????????

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, 1);
//  HAL_UART_Receive_IT(&huart2, (uint8_t *)rx_buffer2, 1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);


    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);


    Arm_Init();

    PIDController PID_x;
    PIDController PID_y;
    PIDController_Init(offset_x, offset_y, &PID_x, &PID_y);
    // 设置时间步长（假设时间步长为0.1秒，�?????????????10000微秒�?????????????
    int dt = 10000;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      if (OpenMV_Rx_Data_Analysis_State)
      {

          OpenMV_Rx_Data_Analysis_State = 0;

          //printf("%d,%d,%d\r\n",duoji_1, duoji_2,duoji_3);

          chasu = PID_Update(&PID_x,dt,OpenMV_Rx_Data[0]);
          //调整舵机的值
//          duoji_1 += -(OpenMV_Rx_Data[0]==1)*OpenMV_Rx_Data[1]+(OpenMV_Rx_Data[0]==0)*OpenMV_Rx_Data[1];
//          duoji_2 += -(OpenMV_Rx_Data[2]==1)*OpenMV_Rx_Data[3]+(OpenMV_Rx_Data[2]==0)*OpenMV_Rx_Data[3];
//          duoji_3 += -(OpenMV_Rx_Data[4]==1)*OpenMV_Rx_Data[5]+(OpenMV_Rx_Data[4]==0)*OpenMV_Rx_Data[5];
            //机械臂动作
            if(OpenMV_Rx_Data[0]==1)Arm_Shen();
            else if(OpenMV_Rx_Data[0]==2)Arm_Suo();
            else if(OpenMV_Rx_Data[0]==3)Arm_Jia();
            else if(OpenMV_Rx_Data[0]==4)Arm_Song();
          printf("%d\r\n",chasu);
          memset(OpenMV_Rx_Data, 0, sizeof(OpenMV_Rx_Data));
      }
      else OpenMV_Check_Data_Task();

      if(xunji_status==0)
      {
          //正常循迹状�?�代码实�?
      }

      else if(xunji_status==1)
      {

      }
      if (OpenMV_Rx_Data_Analysis_State2)
      {

          //下面是对无线串口采集到的数据进行处理的函数，每次接收到一次有效数据之后处理一�??

          chasu = PID_Update(&PID_x,dt,OpenMV_Rx_Data2[0]);
          // 发送数据
          char txData[] = "Hello, UART!\r\n";
          HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
//              if(OpenMV_Rx_Data2[2]<20)
//              {
//                  stop_flag=0;
//                  if(OpenMV_Rx_Data2[1]<3)chasu_flag=0;
//              }

          OpenMV_Rx_Data_Analysis_State2 = 0;
          memset(OpenMV_Rx_Data2, 0, sizeof(OpenMV_Rx_Data2));
      }
      else OpenMV_Check_Data_Task2();
      Forward(setspeed*stop_flag+chasu*chasu_flag,setspeed*stop_flag-chasu*chasu_flag);//输入左轮 右轮的�?�度
//      Forward(chasu,-chasu);//输入左轮 右轮的�?�度
//      Forward(500,500);//输入左轮 右轮的�?�度

      HAL_Delay(100);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
