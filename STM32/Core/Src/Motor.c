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
函数名称：forward(u16 speed)
函数功能：小车前进
入口参数：speed  0-500
返回参数：无
***************************************************/
void forward(u16 speed)
{
    TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
    R_AIN2_ON;

    TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
    R_BIN2_OFF;

    TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
    L_AIN2_OFF;

    TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
    L_BIN2_ON;
}


/**************************************************
函数名称：backward(u16 speed)
函数功能：小车后退
入口参数：speed  0-500
返回参数：无
***************************************************/
void backward(u16 speed)
{
    TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
    R_BIN2_ON;

    TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
    L_AIN2_ON;

    TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
    L_BIN2_OFF;
}


/**************************************************
函数名称：Left_Turn(u16 speed)
函数功能：小车左转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Left_Turn(u16 speed)
{
    TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
    R_AIN2_ON;

    TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
    R_BIN2_ON;

    TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
    L_AIN2_OFF;

    TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
    L_BIN2_OFF;
}


/**************************************************
函数名称：Right_Turn(u16 speed)
函数功能：小车右转
入口参数：speed  0-500
返回参数：无
***************************************************/
void Right_Turn(u16 speed)
{
    TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
    R_BIN2_OFF;

    TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
    L_AIN2_ON;

    TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
    L_BIN2_ON;
}


/************************************************************************
函数名称：Move(u16 Dir,u16 speed)
函数功能：小车平移
入口参数：Dir 平移方向(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					方向 speed  0-500
返回参数：无
*********************************************************&&*************/
void Move(u16 Dir,u16 speed)
{
    if(Dir==0)//左移
    {
        TIM_SetCompare1(TIM2,speed);//R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2,speed);//R_BIN2:左下
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,speed);//L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,speed);//L_BIN2:左上
        L_BIN2_OFF;
    }
    else if(Dir==1)//右移
    {
        TIM_SetCompare1(TIM2,500-speed);//R_AIN2:右下
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,500-speed);//R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM2,500-speed);//L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM2,500-speed);//L_BIN2:左上
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


/**************************************************
函数名称：Motion_State(u16 mode)
函数功能：小车关闭及打开
入口参数：mode (ON OFF)
返回参数：无
***************************************************/
void Motion_State(u16 mode)
{
    if(mode==6)
    {
        L_STBY_ON;
        R_STBY_ON;
        TIM_SetCompare1(TIM2,500);//R_AIN2:右下
        R_AIN2_ON;

        TIM_SetCompare2(TIM2,0);//R_BIN2:左下
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2,0);//L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2,500);//L_BIN2:左上
        L_BIN2_ON;
    }
    else if(mode==7)
    {
        L_STBY_OFF;
        R_STBY_OFF;
    }
}