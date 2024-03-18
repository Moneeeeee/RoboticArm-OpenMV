//
// Created by Monee on 2024/3/18.
//

#ifndef STM32_MOTOR_H
#define STM32_MOTOR_H
#include "main.h"

void Motor_Init(void);
void forward(uint16_t speed);
void backward(uint16_t speed);
void left_turn(uint16_t speed);
void right_turn(uint16_t speed);
void move(uint16_t direction, uint16_t speed);
void motion_state(uint16_t mode);


#endif //STM32_MOTOR_H
