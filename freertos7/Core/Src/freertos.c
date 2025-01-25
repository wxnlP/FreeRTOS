/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "freertos_ITF.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static QueueSetHandle_t g_InputQueueSetHandle;
static QueueHandle_t g_OLEDQueue;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 64,
        .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void QueueSet_Init(void);

void OLED_Queue_Task(void *argument);

void Input_Queue_Task(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */
    QueueSet_Init();
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    xTaskCreate(OLED_Queue_Task, "QueueTask1", 128, NULL, osPriorityNormal, NULL);
    xTaskCreate(Input_Queue_Task, "QueueTask2", 128, NULL, osPriorityNormal, NULL);

    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    rtMotor_Test();
//    rtVoltage_oled_Test();
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void OLED_Queue_Task(void *argument) {
    /* 定义结构 */
    xOLEDData OLED_Queue_Data;
    while (1) {
        /* 读队列 */
        if (pdTRUE == xQueueReceive(g_OLEDQueue, &OLED_Queue_Data, portMAX_DELAY)) {
            OLED_Clear();
            /* 显示队列数据 */
            OLED_ShowNum(0, 0, OLED_Queue_Data.value1, 4, 16);
            OLED_ShowStr(40, 0, OLED_Queue_Data.messages, 16);
            OLED_ShowNum(0, 2, OLED_Queue_Data.value2, 4, 16);
        }
//        vTaskDelay(200);

    }
}

void Input_Queue_Task(void *argument) {
    QueueHandle_t Result_QueueHandel;
    /* 定义结构体 */
    xGPIOData GPIO_Queue_Data;
    xEncoderData Encoder_Queue_Data;
    xOLEDData OLED_Queue_Data;
    while (1) {
        /* 读队列集，得到队列句柄 */
        Result_QueueHandel = xQueueSelectFromSet(g_InputQueueSetHandle, portMAX_DELAY);
        if (Result_QueueHandel) {
            if (Result_QueueHandel == GPIO_GetQueueHandle()) {
                /* 读队列句柄，得到数据 */
                xQueueReceive(Result_QueueHandel, &GPIO_Queue_Data, 0);
                /* 处理数据 */
                OLED_Queue_Data.value1 = GPIO_Queue_Data.count;
                OLED_Queue_Data.value2 = GPIO_Queue_Data.sum;
                strcpy(OLED_Queue_Data.messages, "None");
            } else if (Result_QueueHandel == Encoder_GetQueueHandle()) {
                /* 读队列句柄，得到数据 */
                xQueueReceive(Result_QueueHandel, &Encoder_Queue_Data, 0);
                /* 处理数据 */
                OLED_Queue_Data.value1 = Encoder_Queue_Data.speed;
                OLED_Queue_Data.value2 = Encoder_Queue_Data.value;
                strcpy(OLED_Queue_Data.messages, Encoder_Queue_Data.device);
            }
            /* 写OLED队列 */
            xQueueSend(g_OLEDQueue, &OLED_Queue_Data, 0);
        }
    }
}

/* 初始化队列集 */
void QueueSet_Init(void) {
    QueueHandle_t GPIO_QueueHandle;
    QueueHandle_t Encoder_QueueHandle;
    /* 获取队列句柄 */
    GPIO_QueueHandle = GPIO_GetQueueHandle();
    Encoder_QueueHandle = Encoder_GetQueueHandle();
    /* 创建队列柄 */
    g_InputQueueSetHandle = xQueueCreateSet(Encoder_QUEUE_LENGTH + GPIO_QUEUE_LENGTH);
    /* 将队列加入队列集 */
    xQueueAddToSet(GPIO_QueueHandle, g_InputQueueSetHandle);
    xQueueAddToSet(Encoder_QueueHandle, g_InputQueueSetHandle);
    /* 创建OLED队列 */
    g_OLEDQueue = xQueueCreate(10, sizeof(xOLEDData));
}
/* USER CODE END Application */

