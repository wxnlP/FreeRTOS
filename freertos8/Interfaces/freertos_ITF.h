#ifndef ROBOT_FREERTOS_ITF_H
#define ROBOT_FREERTOS_ITF_H

#include "buzzer.h"
#include "oled.h"
#include "motor.h"
#include "encoder.h"
#include "voltage.h"
#include "uart.h"
#include "timer.h"
#include "game.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


void rtLED_Flash(uint16_t period);
void rtBuzzer_Flash(uint16_t period);
void rtBuzzer_ON_Time(void);
void rtVoltage_uart_Test(void);
void rtVoltage_oled_Test(void);
void rtMotor_Test(void);


#endif //ROBOT_FREERTOS_ITF_H
