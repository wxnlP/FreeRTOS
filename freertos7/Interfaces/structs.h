#ifndef ROBOT_STRUCTS_H
#define ROBOT_STRUCTS_H

#include "stm32f1xx_hal.h"
#include "string.h"

typedef struct {
    int32_t value2;
    int16_t value1;
    char messages[16];
} xOLEDData;


#endif //ROBOT_STRUCTS_H
