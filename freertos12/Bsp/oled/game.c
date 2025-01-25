#include "game.h"

static QueueHandle_t g_QueueHandles[10];
static uint8_t g_QueueCnt = 0;

Car g_car[4] = {
        {0, 0, 0},
        {0, 1, 1},
        {0, 2, 2},
        {0, 3, 3},
};

void GameInit(void) {
    OLED_ShowStr(0, 0, "D", 8);
    OLED_ShowStr(0, 1, "D", 8);
    OLED_ShowStr(0, 2, "D", 8);
//    OLED_ShowStr(0, 3, "D", 8);
}

void ShowCar(Car *pCar) {
    OLED_ShowStr(pCar->x, pCar->y, "D", 8);
}

void HideCar(Car *pCar) {
    OLED_ShowStr(pCar->x, pCar->y, " ", 8);
}

/* 注册队列 */
void RegisterQueue(QueueHandle_t QueueHandle_t) {
    g_QueueHandles[g_QueueCnt] = QueueHandle_t;
    g_QueueCnt++;
}

/* 写多个已注册的队列 */
static void QueuesWrite(InputData *idata) {
    int i;
    for ( i = 0 ; i < g_QueueCnt ; i++ ) {
        xQueueSendFromISR(g_QueueHandles[i], idata, NULL);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    /* 定义变量 */
    static int cnt=0;
    static uint64_t last_time = 0;
    uint64_t current_time = system_get_ns()/1000000;
    /* 定义结构体 */
    InputData idata;
    if (current_time-last_time > 200) {
        if (GPIO_Pin == GPIO_PIN_2) {
            last_time = current_time;
            idata.cnt = cnt;
            cnt++;
            if (cnt > 3) {cnt = 0;}
            /* 写四个队列 */
            QueuesWrite(&idata);
        }
    }
    else {
        last_time = current_time;
    }
}
