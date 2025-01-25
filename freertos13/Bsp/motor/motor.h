#ifndef ROBOT_MOTOR_H
#define ROBOT_MOTOR_H

#include "tim.h"

void Motor_Init(void);
void Motor_Stop(void);
void Motor_SetSpeed(uint8_t Mx, float duty);


// 定义死区阈值（占空比），最低的比较值为1900
#define DEAD_ZONE_THRESHOLD     52.78
#define DEAD_ZONE_ARR           1850
#define M1A(duty)               __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, duty)
#define M1B(duty)               __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, duty)
#define M2A(duty)               __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, duty)
#define M2B(duty)               __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_4, duty)
#define M3A(duty)               __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, duty)
#define M3B(duty)               __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, duty)
#define M4A(duty)               __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, duty)
#define M4B(duty)               __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, duty)

#endif //ROBOT_MOTOR_H
