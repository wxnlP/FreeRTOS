#include "encoder.h"


/* 定义队列 */
static QueueHandle_t g_Encoder_Queue;

/* @brief  编码器接口初始化（STM32默认开启，不初始化也可以读编码器值）
 * @param  无参数
 * @retval 无返回值
 */
void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    HAL_TIM_Base_Start_IT(&htim7);
    /* 编码器初始化时，创建专属队列 */
    g_Encoder_Queue = xQueueCreate(Encoder_QUEUE_LENGTH, sizeof(xEncoderData));
}

/* @brief  读取编码器值，并手动清零
 * @param  Hx: 选择电机编码器H1左上,H2左下,H3右上,H4右下
 * @retval 返回定时器计数值
 */
int16_t Encoder_Read_CNT(uint8_t Hx)
{
    int16_t value;
    switch (Hx) {
        case 1:
            value = __HAL_TIM_GET_COUNTER(&htim2);
            __HAL_TIM_SET_COUNTER(&htim2, 0);
            break;
        case 2:
            value = __HAL_TIM_GET_COUNTER(&htim4);
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            break;
        case 3:
            value = __HAL_TIM_GET_COUNTER(&htim5);
            __HAL_TIM_SET_COUNTER(&htim5, 0);
            break;
        case 4:
            value = __HAL_TIM_GET_COUNTER(&htim3);
            __HAL_TIM_SET_COUNTER(&htim3, 0);
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

/* 编码器中断功能函数 */
void Encoder_Func_IRQ(void) {
    /* 定义结构体 */
    xEncoderData Encoder_Queue_Data;
    /* 初始化数据 */
    Encoder_Queue_Data.value = Encoder_Read_CNT(4);
    Encoder_Queue_Data.speed = (int16_t)(Encoder_Queue_Data.value % 100);
    strcpy(Encoder_Queue_Data.device, "M4");
    /* 写Encoder队列 */
    xQueueSendFromISR(g_Encoder_Queue, &Encoder_Queue_Data, NULL);
}

/* 外部文件查看队列句柄 */
QueueHandle_t Encoder_GetQueueHandle(void) {
    return g_Encoder_Queue;
}

