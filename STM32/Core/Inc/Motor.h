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
#endif //STM32_MOTOR_H
