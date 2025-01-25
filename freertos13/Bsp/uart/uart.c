#include "uart.h"

// 将整数转换为字符串并通过串口发送
void UART_SendInt16(UART_HandleTypeDef *huart, int16_t num) {
    char str[10];  // 用来存储数字的字符串表示
    int i = 0;
    // 处理负数
    if (num < 0) {
        str[i++] = '-';  // 添加负号
        num = -num;      // 将数字转换为正数
    }
    // 将整数转换为字符串
    sprintf(&str[i], "%d", num);  // 将数字转换为字符串
    // 通过串口发送字符串
    HAL_UART_Transmit(huart, (uint8_t*)str, strlen(str), 1000);  // 发送字符串
}