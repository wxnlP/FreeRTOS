#ifndef FREERTOS1_GAME_H
#define FREERTOS1_GAME_H

#include "oled.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct {
   uint8_t x;
   uint8_t y;
   uint8_t dev;
} Car;

typedef struct {
    uint8_t cnt;
} InputData;

void GameInit(void);
void ShowCar(Car *pCar);
void HideCar(Car *pCar);
void RegisterQueue(QueueHandle_t QueueHandle_t);


#endif //FREERTOS1_GAME_H
