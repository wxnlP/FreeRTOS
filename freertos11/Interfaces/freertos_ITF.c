#include "freertos_ITF.h"

static TimerHandle_t g_SoftTimerHandle;

static void rtLED_Flash()
{
    LED_ON();
    vTaskDelay(100);
    LED_OFF();
    vTaskDelay(100);
}

static void rtBuzzer_Flash()
{
    Buzzer_ON();
    vTaskDelay(50);
    Buzzer_OFF();
}

/* 电池警告模块，可通过按键关闭蜂鸣器 */
void rtBuzzer_ON_Time(void)
{
    static uint8_t status = 1;
    if (status == 1) {
        rtBuzzer_Flash();
    }
    rtLED_Flash();
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
        Motor_Stop();
        vTaskDelay(2000);
    }
}

/* 软件定时器超时函数 */
static void rtCallbackFunc(TimerHandle_t xTimer) {
    Buzzer_OFF();
}

/* 软件定时器控制蜂鸣器 */
void rtBuzzerSoftTimer_Init(void) {
    g_SoftTimerHandle = xTimerCreate(
            "BuzzerSound",
            200,
            pdFALSE,
            NULL,
            rtCallbackFunc
            );
}

/* 调用蜂鸣器，开启定时器 */
void rtBuzzerON(TickType_t period) {
    Buzzer_ON();
    xTimerChangePeriod(g_SoftTimerHandle, period, 0);
}

