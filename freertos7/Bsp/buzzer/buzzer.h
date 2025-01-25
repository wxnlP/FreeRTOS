#ifndef BUZZER_BUZZER_H
#define BUZZER_BUZZER_H

#include "gpio.h"
#include "encoder.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "queue.h"

#define GPIO_QUEUE_LENGTH   10

typedef struct {
    uint8_t count;
    uint16_t sum;
}xGPIOData;

void LED_Flash(int period);
void Buzzer_Flash(int period);
void Buzzer_ON_Time(void);
uint8_t Get_KeyNum(void);

void GPIO_QueueInit(void);
QueueHandle_t GPIO_GetQueueHandle(void);

// define
#define LED_ON()      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
#define LED_OFF()     HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define Buzzer_ON()   HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_SET)
#define Buzzer_OFF()  HAL_GPIO_WritePin(buzzer_GPIO_Port, buzzer_Pin, GPIO_PIN_RESET)
#define Key_Status    HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)

#endif //BUZZER_BUZZER_H
