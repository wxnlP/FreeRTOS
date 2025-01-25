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
// 定义队列句柄
QueueHandle_t g_QueueA = NULL;
QueueHandle_t g_QueueB = NULL;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 128,
        .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void OLED_Queue_Task(void *argument);

void Encoder_Queue_Task(void *argument);

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
    /* 创建队列 */
    g_QueueA = xQueueCreate(10, sizeof(OLEDData));
    g_QueueB = xQueueCreate(10, sizeof(EncoderData));
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
    xTaskCreate(Encoder_Queue_Task, "QueueTask2", 128, NULL, osPriorityNormal, NULL);

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
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void OLED_Queue_Task(void *argument) {
    /* 定义结构 */
    OLEDData QueueB_Data_Output;
    while (1) {
        /* 读队列B */
        if (pdTRUE == xQueueReceive(g_QueueB, &QueueB_Data_Output, portMAX_DELAY)) {
            /* 显示队列B数据 */
            OLED_ShowNum(0, 0, QueueB_Data_Output.speed, 4, 16);
            OLED_ShowStr(40, 0, QueueB_Data_Output.messages, 16);
            OLED_ShowNum(0, 2, QueueB_Data_Output.cnt_value, 4, 16);
        }
        vTaskDelay(200);
    }
}

void Encoder_Queue_Task(void *argument) {
    /* 定义结构 */
    EncoderData QueueA_Data_Output;
    OLEDData QueueB_Data_Input;
    while (1) {
        /* 读队列A */
        if (pdTRUE == xQueueReceive(g_QueueA, &QueueA_Data_Output, portMAX_DELAY)) {
            /* 处理队列A数据 */
            QueueB_Data_Input.cnt_value = (int16_t) (QueueA_Data_Output.cnt_value - 1000);
            QueueB_Data_Input.speed = (QueueA_Data_Output.speed) % 10;
            strcpy(QueueB_Data_Input.messages, "SendOLED!");
//            OLED_ShowNum(0, 0, QueueB_Data_Input.speed, 4, 16);
//            OLED_ShowStr(40, 0, QueueB_Data_Input.messages, 16);
//            OLED_ShowNum(0, 2, QueueB_Data_Input.cnt_value, 4, 16);
            /* 发送队列B */
            if (pdTRUE == xQueueSend(g_QueueB, &QueueB_Data_Input, 0)) {
                LED_ON();
            }
        }
        vTaskDelay(200);
    }
}
/* USER CODE END Application */

