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
extern Car g_car[4];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static TaskHandle_t g_Task2Handle;
static TaskHandle_t g_Task3Handle;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void OLED_PrintTask1(void *params);

void OLED_PrintTask2(void *params);

void OLED_PrintTask3(void *params);

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
    xTaskCreate(OLED_PrintTask1, "Task1", 128, &g_car[0], osPriorityNormal, NULL);
    xTaskCreate(OLED_PrintTask2, "Task2", 128, &g_car[1], osPriorityNormal + 2, &g_Task2Handle);
    xTaskCreate(OLED_PrintTask3, "Task3", 128, &g_car[2], osPriorityNormal + 2, &g_Task3Handle);

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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void OLED_PrintTask1(void *params) {
    Car *car = params;
    ShowCar(car);
    while (1) {
        HideCar(car);
        car->x += 1;
        if (car->x > 128) { car->x = 120; }
        ShowCar(car);
        vTaskDelay(50);
        if (car->x == 120) {
            /* 给Task2发�?�两次任务�?�知，使得�?�知值为2，模拟计数型信号�? */
            xTaskNotifyGive(g_Task2Handle);
            xTaskNotifyGive(g_Task2Handle);
            /* 给Task3发�?�任务�?�知�?100（eSetValueWithOverwrite操作�? */
            xTaskNotify(g_Task3Handle, 100, eSetValueWithOverwrite);
            /* 设置事件 */
            vTaskDelete(NULL);
        }
    }
}

void OLED_PrintTask2(void *params) {
    Car *car = params;
    uint8_t cnt;
    ShowCar(car);
    /* 等待任务通知，将通知值清零，模拟二进制信号量 */
    cnt = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    while (1) {
        HideCar(car);
        car->x += 1;
        if (car->x > 128) { car->x = 120; }
        ShowCar(car);
        vTaskDelay(100);
        if (car->x == 120) {
            OLED_ShowNum(0, 0, cnt, 1, 16);
            vTaskDelete(NULL);
        }
    }
}

void OLED_PrintTask3(void *params) {
    uint32_t value;
    Car *car = params;
    ShowCar(car);
    do {
        /* 等待特定的�?�知�? */
        xTaskNotifyWait(0, 0, &value, portMAX_DELAY);
    } while (value != 100);
    while (1) {
        HideCar(car);
        car->x += 1;
        if (car->x > 128) { car->x = 120; }
        ShowCar(car);
        vTaskDelay(100);
//        HAL_Delay(100);
        if (car->x == 120) {
            vTaskDelete(NULL);
        }
    }
}
/* USER CODE END Application */

