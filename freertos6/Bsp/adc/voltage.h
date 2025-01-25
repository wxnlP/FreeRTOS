#ifndef ROBOT_VOLTAGE_H
#define ROBOT_VOLTAGE_H

#include "adc.h"
#include "../../Bsp/uart/uart.h"
#include "../../Bsp/oled/oled.h"

void ADC_Voltage_Init(void);
uint16_t ADC_Voltage_Analog(void);
double ADC_Voltage_Actual(void);
void OLED_ShowVoltage(uint8_t x, uint8_t y, double num, uint8_t fontsize);

#endif //ROBOT_VOLTAGE_H
