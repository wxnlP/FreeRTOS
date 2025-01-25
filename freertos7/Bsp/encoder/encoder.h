#ifndef ROBOT_ENCODER_H
#define ROBOT_ENCODER_H

#include <string.h>
#include "tim.h"
#include "FreeRTOS.h"
#include "queue.h"

#define Encoder_QUEUE_LENGTH   10

typedef struct {
    char device[6];
    int32_t value;
    int16_t speed;
}xEncoderData;


void Encoder_Init(void);
int16_t Encoder_Read_CNT(uint8_t Hx);
void Encoder_Func_IRQ(void);
QueueHandle_t Encoder_GetQueueHandle(void);

#endif //ROBOT_ENCODER_H
