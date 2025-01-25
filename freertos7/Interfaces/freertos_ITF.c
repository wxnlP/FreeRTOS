#include "freertos_ITF.h"


void rtLED_Flash(uint16_t period)
{
    LED_ON();
    vTaskDelay(period);
    LED_OFF();
    vTaskDelay(period);
}

void rtBuzzer_Flash(uint16_t period)
{
    Buzzer_ON();
    vTaskDelay(period);
    Buzzer_OFF();
}

/* 电池警告模块，可通过按键关闭蜂鸣器 */
void rtBuzzer_ON_Time(void)
{
    static uint8_t status = 1;
    if (status == 1) {
        rtBuzzer_Flash(50);
    }
    rtLED_Flash(100);
    if (Key_Status == 0){
        while (Key_Status == 0);
        Buzzer_OFF();
        status = 0;
    }
}

/* 电池电压测量函数,串口查看 */
void rtVoltage_uart_Test(void)
{
    uint16_t analog_value = 0;
    double voltage = 0.0;
    char message[25] = "";
    while (1)
    {
        analog_value = ADC_Voltage_Analog();
        voltage = ADC_Voltage_Actual();
        sprintf(message, "ADC: %hu   %.2fV\r\n", analog_value, voltage);
        HAL_UART_Transmit(&huart1, (uint8_t *) message, strlen(message), HAL_MAX_DELAY);
        vTaskDelay(500);
    }
}



/* 电池电压测量函数,OLED查看 */
void rtVoltage_oled_Test(void)
{
    double voltage = 0.0;
    while (1)
    {
        voltage = ADC_Voltage_Actual();
        OLED_ShowVoltage(88, 2, voltage, 16);
        vTaskDelay(200);
    }
}

/* 电机测试函数 */
void rtMotor_Test(void)
{
//    Motor_Stop();
    while(1)
    {
        Motor_SetSpeed(4, 20);
        vTaskDelay(2000);
//        Motor_Stop();
//        vTaskDelay(2000);
    }
}


