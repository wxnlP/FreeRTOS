#ifndef ROBOT_STRUCTS_H
#define ROBOT_STRUCTS_H

#include "stm32f1xx_hal.h"
#include "string.h"

typedef struct{
    int32_t speed;
    int16_t cnt_value;
    char messages[16];
}EncoderData;

typedef struct{
    int32_t speed;
    int16_t cnt_value;
    char messages[16];
}OLEDData;


#endif //ROBOT_STRUCTS_H
