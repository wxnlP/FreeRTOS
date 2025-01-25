#ifndef ROBOT_UART_H
#define ROBOT_UART_H

#include "usart.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void UART_SendInt16(UART_HandleTypeDef *huart, int16_t num);

#endif //ROBOT_UART_H
