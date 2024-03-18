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

/**************************************************
函数名称：forward(uint16_t speed)
函数功能：小车前进
入口参数：speed  0-500
返回参数：无
***************************************************/
void forward(uint16_t speed)
{

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);//R_AIN2:右下
    R_AIN2_ON;//
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_BIN2:左下
    R_BIN2_OFF;//
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);//L_AIN2:右上
    L_AIN2_OFF;//
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500 - speed);//L_BIN2:左上
    L_BIN2_ON;//

}


/**************************************************
函数名称：backward(uint16_t speed)
函数功能：小车后退
入口参数：speed  0-500
返回参数：无
***************************************************/
void backward(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);//R_AIN2:右下
    R_AIN2_OFF;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_BIN2:左下
    R_BIN2_ON;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);//L_AIN2:右上
    L_AIN2_ON;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);//L_BIN2:左上
    L_BIN2_OFF;

}


/**************************************************
函数名称：Left_Turn(uint16_t speed)
函数功能：小车左转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Left_Turn(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);//R_AIN2:右下
    R_AIN2_ON;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_BIN2:左下
    R_BIN2_ON;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed);//L_AIN2:右上
    L_AIN2_OFF;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed);//L_BIN2:左上
    L_BIN2_OFF;

}


/**************************************************
函数名称：Right_Turn(uint16_t speed)
函数功能：小车右转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Right_Turn(uint16_t speed)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);//R_AIN2:右下
    R_AIN2_OFF;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);//R_BIN2:左下
    R_BIN2_OFF;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500-speed));//L_AIN2:右上
    L_AIN2_ON;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500-speed));//L_BIN2:左上
    L_BIN2_ON;

}


/************************************************************************
函数名称：Move(uint16_t Dir,uint16_t speed)
函数功能：小车平移
入口参数：Dir 平移方向(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					方向 speed  0-500
返回参数：无
*********************************************************&&*************/
void Move(uint16_t Dir,uint16_t speed)
{
    if(Dir==0)//左移
    {


        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, speed);//R_AIN2:右下
        R_AIN2_OFF;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, speed);//R_BIN2:左下
        R_BIN2_OFF;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, speed));//L_AIN2:右上
        L_AIN2_OFF;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, speed));//L_BIN2:左上
        L_BIN2_OFF;

    }
    else if(Dir==1)//右移
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500 - speed);//R_AIN2:右下
        R_AIN2_ON;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 500 - speed);//R_BIN2:左下
        R_BIN2_ON;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500 - speed);//L_AIN2:右上
        L_AIN2_ON;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 500 - speed);//L_BIN2:左上
        L_BIN2_ON;
    }
    else if(Dir==2)//左上移动
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
        L_BIN2_OFF;
    }
    else if(Dir==3)//右上移动
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
        L_BIN2_ON;
    }
    else if(Dir==4)//左下移动
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
        L_BIN2_OFF;
    }
    else if(Dir==5)//右下移动
    {
        TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
        L_BIN2_OFF;
    }
}

