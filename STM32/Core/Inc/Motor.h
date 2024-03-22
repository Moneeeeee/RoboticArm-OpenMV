//
// Created by Monee on 2024/3/18.
//

#ifndef STM32_MOTOR_H
#define STM32_MOTOR_H
#include "main.h"
#include "string.h"




void Motor_Init(void);
void Translate_Move(char *Dir, uint16_t speed) ;
//void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d,int servo);

void Front_Left(uint8_t state);
void Front_Right(uint8_t state);
void Back_Left(uint8_t state);
void Back_Right(uint8_t state);
void Forward(uint16_t speed);
void Backward(uint16_t speed);
void Left_Turn(uint16_t speed);
void Right_Turn(uint16_t speed);
void Translate_Move(char *Dir, uint16_t speed);

// PID 控制器状态
typedef struct {
    int target_position;    // 目标位置
    int current_position;   // 当前位置
    int prev_error;         // 前一次误差
    int integral;           // 积分项
} PIDController;


int PID_Update(PIDController *pid, int dt);
void PIDController_Init(int member1, int member2, PIDController *PID_x, PIDController *PID_y);
#endif //STM32_MOTOR_H
