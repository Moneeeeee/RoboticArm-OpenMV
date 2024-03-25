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
    // 停止定时�???
    HAL_TIM_Base_Stop(htim);

    // 修改预分频系�???
    htim->Init.Prescaler = newPrescaler;

    // 重新初始化定时器
    HAL_TIM_Base_Init(htim);

    // 重新启动定时�???
    HAL_TIM_Base_Start(htim);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define Target_Speed 500

extern uint8_t OpenMV_Rx_Data_Analysis_State;
extern short OpenMV_Rx_Data[6];             // 接收OpenMV数据

int Length_Flag = 12;
int Turn_Out = 0;
int Length = 0;
int err_x = 0;
int Deviation = 0;
int Para_Out = 0;
int Paralell_Flag = 0;//巡线后�?�抓取前之间的平移窗�?
int State = 1;//分为抓取模式�?0）和循迹模式�?1�?
int Turn_Flag=1;//是否给差速�?�巡线时启用
int Stop_Flag=1;//巡线结束停车、放下物体停�?
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  RetargetInit(&huart1);

  uint8_t rx_buffer[BUFFER_SIZE];  // 声明接收缓冲�??????????????

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
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_buffer, 1);

//  //轮子PWM初始�?
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

    //舵机PWM初始�?
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
    //机械臂初始收�?
    Arm_Init();

    //PID结构体定义（仅对巡线差�?�）
    PIDController PID_x;
    PIDController_Init(err_x,&PID_x);

    PIDController PID_d;
    PIDController_Init(Deviation,&PID_d);

//    //没有必要？没有必�?
//        // 设置时间步长（假设时间步长为100us�?
//        int dt = 100;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      Translate_Move("Left",Para_Out);

      if (OpenMV_Rx_Data_Analysis_State)
      {
          OpenMV_Rx_Data_Analysis_State = 0;
          //�?共六位�?�第�?位为舵机控制、第二位为x偏差、第三位为距离�?�第四位为元素识别标志位、第五位为中线斜率�?�第六位0
          //调整舵机的�??
//          duoji_1 += -(OpenMV_Rx_Data[0]==1)*OpenMV_Rx_Data[1]+(OpenMV_Rx_Data[0]==0)*OpenMV_Rx_Data[1];
//          duoji_2 += -(OpenMV_Rx_Data[2]==1)*OpenMV_Rx_Data[3]+(OpenMV_Rx_Data[2]==0)*OpenMV_Rx_Data[3];
//          duoji_3 += -(OpenMV_Rx_Data[4]==1)*OpenMV_Rx_Data[5]+(OpenMV_Rx_Data[4]==0)*OpenMV_Rx_Data[5];

            //第一位为舵机控制
            //OpenMV_Rx_Data[0]
//            if(OpenMV_Rx_Data[0]==1)Arm_Shen();
//            else if(OpenMV_Rx_Data[0]==2)Arm_Suo();
//            else if(OpenMV_Rx_Data[0]==3)Arm_Jia();
//            else if(OpenMV_Rx_Data[0]==4)Arm_Song();

            //第1位为x偏差
            //OpenMV_Rx_Data[1]
            err_x = OpenMV_Rx_Data[0];
            Turn_Out = PID_Update(&PID_x,err_x);
//            printf("%d,%d\r\n",OpenMV_Rx_Data[1],Turn_Out);

            //第2位为距离、Length_Flag为标记距�?
            //OpenMV_Rx_Data[2]
            Length = OpenMV_Rx_Data[1];
            if(Length - Length_Flag<0){
                Paralell_Flag = 1;
                Forward(200,200);
                //执行平移操作
                //执行抓取操作
            }

            //第3位为元素识别标志
            //OpenMV_Rx_Data[3]
            Stop_Flag = OpenMV_Rx_Data[2];
            if(Stop_Flag){
                //识别到之后，�?始进行抓�?
                State = 0;
                //左右平移（根据x偏差�?
                //对准�? 进行Length判断
            }

          memset(OpenMV_Rx_Data, 0, sizeof(OpenMV_Rx_Data));
      }
      else OpenMV_Check_Data_Task();


      //夹取物体状�??
      if(State==0)
      {
          //平移
          if(Paralell_Flag == 1){

              Forward(0,0);
              //平移标志�? = 1�?
              //夹取动作�?
              //�?
              Arm_Shen();
              //前进�?段距�?
              Forward(200,200);
              HAL_Delay(1000);
              //夹取
              Arm_Jia();
              HAL_Delay(1000);

              Arm_Suo();
              //停止标志位�?�然后继续进入巡�?
              State  = 1;
              Paralell_Flag = 0;
          }
          else{

              err_x<0?-err_x:err_x;
              if(err_x> 20){

                  Para_Out = PID_Update(&PID_x,err_x);
                  Para_Out>0?Para_Out:-Para_Out;//绝对�?

                  if(err_x > 0){
                      Translate_Move("Left",Para_Out);
                  }
                  else if(err_x < 0){
                      Translate_Move("Right",Para_Out);
                  }
              }
          }
      }
      //巡线状�??
      else if(State==1)
      {

          //误差压入PID，给出差�?
          //这里不能使用err_x了，而是应该用中线斜�?
          int Left_Sensor = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2);
          int Right_Sensor = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3);

          if(Left_Sensor==0&&Right_Sensor==0){
                            Forward(300,300);
//              State = 0;

          }
          else if(Left_Sensor==1&&Right_Sensor==1)Forward(300,300);
          else if(Left_Sensor==0&&Right_Sensor==1)Forward(0,300);
          else if(Left_Sensor==1&&Right_Sensor==0)Forward(300,0);

            //当识别到元素

      }


      //防打架Delay
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
