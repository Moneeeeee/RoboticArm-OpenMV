//
// Created by Monee on 2024/3/18.
//
#include "Motor.h"
//duoji2:前伸舵机。duoji3:抓取舵机
//前伸未抓取状态duoji2:210,duoji3:250
//前伸抓取状态duoji2:210,duoji3:170
//后缩状态duoji2:70
//机械臂初始化
int step_shen = 70;
int step_zhua =  245;
void Arm_Init(void){
    //初始
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, step_shen);//后
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, step_zhua);//松
    HAL_Delay(1000);

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 160);// 下
    HAL_Delay(1000);

}
//机械臂前伸
void Arm_Shen(void){

    while(step_shen<210)
    {
        step_shen +=5;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, step_shen);//后
        HAL_Delay(20);
    }

}
void Arm_Suo(void){

    while(step_shen>70)
    {
        step_shen -=5;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, step_shen);//后
        HAL_Delay(20);
    }

}
//夹取
void Arm_Jia(void){
    while(step_zhua>180)
    {
        step_zhua -=5;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, step_zhua);//松
        HAL_Delay(10);
    }
}

//松爪子
void Arm_Song(void){

    while(step_zhua<245)
    {
        step_zhua +=5;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, step_zhua);//松
        HAL_Delay(10);
    }

}
/**************************************************
函数名称：Motor_Init(void)
函数功能：电机相关引脚初始化
入口参数：无
返回参数：无
***************************************************/

void Back_Left(uint8_t state){
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

void Back_Right(uint8_t state){
    if(state == 1){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_RESET);
    }

    else if(state == 0){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_SET);
    }
}

void Front_Left(uint8_t state){
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
void Forward(uint16_t L_speed ,uint16_t R_speed)
{

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, R_speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, L_speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, L_speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, R_speed);//L_B:左上

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
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);//L_B:左上

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
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);//L_B:左上

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
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);//R_A:右下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);//R_B:左下
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);//L_A:右上
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);//L_B:左上

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
    if(strcmp(Dir, "Right") == 0) {
        // 右边移动的逻辑
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);
        Front_Left(1);
        Front_Right(0);
        Back_Left(0);
        Back_Right(1);
    }
    else if(strcmp(Dir, "Left") == 0) {
        // 左移动的逻辑
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);
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
int PID_Update(PIDController *pid, int dt,int err) {
    // 计算误差
//    int error = pid->target_position - pid->current_position;

    int error = err;
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


void Servo_1_SetAngle(uint8_t angle)//   input 0~180
{
    // 将角度转换为对应的脉冲宽度
    uint16_t pulse_width = 500 + (angle * 11); // 将角度映射到500µs到2500µs范围内

    // 设置PWM脉冲宽度
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width);
}

void Servo_2_SetAngle(uint8_t angle)
{
    // 将角度转换为对应的脉冲宽度
    uint16_t pulse_width = 500 + (angle * 11); // 将角度映射到500µs到2500µs范围内

    // 设置PWM脉冲宽度
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pulse_width);
}


void Servo_3_SetAngle(uint8_t angle)
{
    // 将角度转换为对应的脉冲宽度
    uint16_t pulse_width = 500 + (angle * 11); // 将角度映射到500µs到2500µs范围内

    // 设置PWM脉冲宽度
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pulse_width);
}


void Steer_Angle(uint8_t angle)
{

}

#define SERVO_LEFT   600
#define SERVO_RIGHT  600
#define SERVO_CARRY  600

void Steer_Init(void)
{

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);


    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,SERVO_LEFT); //相当于一个周期内（20ms）有0.5ms高脉冲

    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,SERVO_RIGHT); //相当于一个周期内（20ms）有0.5ms高脉冲

    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,SERVO_CARRY); //相当于一个周期内（20ms）有0.5ms高脉冲



}




void Steer_Control(uint8_t forword,uint8_t height,uint8_t carry){

//    //角度转化---->PWM值
//    uint8_t forward_angle = forword * kp;
//    uint8_t height_angle = height * kp;
//    uint8_t carry_angle = carry * kp;
//
//

    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,SERVO_LEFT); //相当于一个周期内（20ms）有0.5ms高脉冲

    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,SERVO_RIGHT); //相当于一个周期内（20ms）有0.5ms高脉冲

    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,SERVO_CARRY); //相当于一个周期内（20ms）有0.5ms高脉冲


}
//
//void  Extend_STEER(){
//
//    //机械臂前伸
//
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//
//}
//
//void  Retract_STEER(){
//
////机械臂后缩
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//
//}
//
//void  Grasp_STEER(){
//
////    机械臂抓取
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//
//}
//
//void  Release_STEER(){
//
//      //    机械臂松开
//    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,WATTINGTOMEASURE); //相当于一个周期内（20ms）有0.5ms高脉冲
//
//
//}