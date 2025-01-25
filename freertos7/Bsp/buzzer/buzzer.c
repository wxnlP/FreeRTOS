#include "buzzer.h"

/* 定义队列 */
static QueueHandle_t g_GPIO_Queue;

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    /* 定义变量 */
    static int cnt=0;
    static uint64_t last_time = 0;
    /* 定义结构体 */
    static xGPIOData GPIO_Queue_Data = {0, 0};
    uint64_t current_time = system_get_ns()/1000000;
    if (current_time-last_time > 200) {
        if (GPIO_Pin == GPIO_PIN_2) {
            last_time = current_time;
            /* 初始化数据 */
            cnt++;
            GPIO_Queue_Data.count = cnt;
            GPIO_Queue_Data.sum += cnt;
            /* 写GPIO队列 */
            xQueueSendFromISR(g_GPIO_Queue, &GPIO_Queue_Data, NULL);
        }
    }
    else {
        last_time = current_time;
    }
}

/* 创建GPIO外部中断所需队列 */
void GPIO_QueueInit(void) {
    g_GPIO_Queue = xQueueCreate(GPIO_QUEUE_LENGTH, sizeof(xGPIOData));
}

/* 外部文件查看队列句柄 */
QueueHandle_t GPIO_GetQueueHandle(void) {
    return g_GPIO_Queue;
}