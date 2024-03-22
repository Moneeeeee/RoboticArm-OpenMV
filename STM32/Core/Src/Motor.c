//
// Created by Monee on 2024/3/18.
//
#include "Motor.h"

/**************************************************
函数名称：Motor_Init(void)
函数功能：电机相关引脚初始化
入口参数：无
返回参数：无
***************************************************/
void Motor_Init(void)
{

}

//void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d,int servo)
//{
//    //Forward and reverse control of motor
//    //电机正反转控制
//    if(motor_a<0)		AIN2=0,		AIN1=1;
//    else				    AIN2=1,		AIN1=0;
//    //Motor speed control
//    //电机转速控制
//    PWMA=abs(motor_a);
//
//    //Forward and reverse control of motor
//    //电机正反转控制
//    if(motor_b<0)		BIN2=1,		BIN1=0;
//    else 	          BIN2=0,		BIN1=1;
//    //Motor speed control
//    //电机转速控制
//    PWMB=abs(motor_b);
//
//    //Forward and reverse control of motor
//    //电机正反转控制
//    if(motor_c>0)		CIN2=0,		CIN1=1;
//    else 	          CIN2=1,		CIN1=0;
//    //Motor speed control
//    //电机转速控制
//    PWMC=abs(motor_c);
//
//    //Forward and reverse control of motor
//    //电机正反转控制
//    if(motor_d>0)		DIN2=0,		DIN1=1;
//    else 	          DIN2=1,		DIN1=0;
//    //Motor speed control
//    //电机转速控制
//    PWMD=abs(motor_d);
//
//    //Servo control
//    //舵机控制
//    Servo_PWM =servo;
//}

void Front_Left(uint8_t state){
    if(state == 1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
    }

    else if(state == 0){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_RESET);
    }
}

void Front_Right(uint8_t state){
    if(state == 1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);
    }

    else if(state == 0){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET);
    }
}

void Back_Left(uint8_t state){
    if(state == 1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_RESET);
    }

    else if(state == 0){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_SET);
    }
}

void Back_Right(uint8_t state){
    if(state == 1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_RESET);
    }

    else if(state == 0){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_RESET);
    }
}
/**************************************************
函数名称：forward(uint16_t speed)
函数功能：小车前进
入口参数：speed  0-500
返回参数：无
***************************************************/
void Forward(uint16_t speed)
{

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500 - speed);//L_B:左上

    Front_Left(1);
    Front_Right(1);
    Back_Left(1);
    Back_Right(1);

}


/**************************************************
函数名称：backward(uint16_t speed)
0000000000000000000000000000000000000000000000000000000000000000000
***************************************************/
void Backward(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);//L_B:左上

    Front_Left(0);
    Front_Right(0);
    Back_Left(0);
    Back_Right(0);

}


/**************************************************
函数名称：Left_Turn(uint16_t speed)
函数功能：小车左转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Left_Turn(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);//L_B:左上

    Front_Left(0);
    Front_Right(1);
    Back_Left(0);
    Back_Right(1);

}


/**************************************************
函数名称：Right_Turn(uint16_t speed)
函数功能：小车右转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Right_Turn(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500-speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500-speed);//L_B:左上

    Front_Left(1);
    Front_Right(0);
    Back_Left(1);
    Back_Right(0);

}


/************************************************************************
函数名称：Move(uint16_t Dir,uint16_t speed)
函数功能：小车平移
入口参数：Dir 平移方向(Left R_Move Right)
					方向 speed  0-500
返回参数：无
*********************************************************&&*************/
void Translate_Move(char *Dir, uint16_t speed) {
    if(strcmp(Dir, "Left") == 0) {
        // 左移动的逻辑
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);
        Front_Left(1);
        Front_Right(0);
        Back_Left(0);
        Back_Right(1);
    }
    else if(strcmp(Dir, "Right") == 0) {
        // 右移动的逻辑
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500 - speed);
        Front_Left(0);
        Front_Right(1);
        Back_Left(1);
        Back_Right(0);
    }
//    else if(strcmp(Dir, "Left_Up") == 0) {
//        // 左上移动的逻辑
//        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);
//        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);
//        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);
//        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500 - speed);
//        Front_Left(0);
//        Front_Right(1);
//        Back_Left(0);
//        Back_Right(0);
//    }
//    // 更多方向的逻辑...
}

// 更新PID控制器状态
int PID_Update(PIDController *pid, int dt) {
    // 计算误差
    int error = pid->target_position - pid->current_position;

    // 计算积分项
    pid->integral += error * dt;

    // 计算微分项
    int derivative = (error - pid->prev_error) / dt;

    // 计算控制输出
    int output = KP * error + KI * pid->integral + KD * derivative;

    // 更新前一次误差
    pid->prev_error = error;

    return output;
}

void PIDController_Init(int member1, int member2, PIDController *PID_x, PIDController *PID_y) {
    // 初始化第一个PIDController结构体的成员变量
    PID_x->target_position = 0;           // 设置目标位置
    PID_x->current_position = member1;    // 设置当前位置
    PID_x->prev_error = 0;                // 初始化前一次误差
    PID_x->integral = 0;                  // 初始化积分项

    // 初始化第二个PIDController结构体的成员变量
    PID_y->target_position = 0;           // 设置目标位置
    PID_y->current_position = member2;    // 设置当前位置
    PID_y->prev_error = 0;                // 初始化前一次误差
    PID_y->integral = 0;                  // 初始化积分项
}