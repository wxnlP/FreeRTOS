#ifndef ROBOT_STRUCTS_H
#define ROBOT_STRUCTS_H

#include "stm32f1xx_hal.h"

typedef struct{
    int32_t speed;
    int16_t cnt_value;
}EncoderData;

typedef struct{
    int32_t speed;
    int16_t cnt_value;
    char message[10];
}OLEDData;


#endif //ROBOT_STRUCTS_H
