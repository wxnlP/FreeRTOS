#include "buzzer.h"

void LED_Flash(int period)
{
    LED_ON();
    HAL_Delay(period);
    LED_OFF();
    HAL_Delay(period);
}

void Buzzer_Flash(int period)
{
    Buzzer_ON();
    HAL_Delay(period);
    Buzzer_OFF();
//    HAL_Delay(period);
}

/* 电池警告模块，可通过按键关闭蜂鸣器 */
void Buzzer_ON_Time(void)
{
    static uint8_t status = 1;
    if (status == 1) {
        Buzzer_Flash(50);
    }
    LED_Flash(100);
    if (Key_Status == 0){
        while (Key_Status == 0);
        Buzzer_OFF();
        status = 0;
    }
}

uint8_t Get_KeyNum(void)
{
    static uint16_t num=0;
    if (Key_Status == 0)
    {
        while (Key_Status==0);
        num++;
    }
    return num%2;
}

