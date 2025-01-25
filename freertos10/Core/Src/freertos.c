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
static SemaphoreHandle_t g_xSemaphoreHandle;
static EventGroupHandle_t g_xEventCarHandle;
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
//    g_xSemaphoreHandle = xSemaphoreCreateMutex();
//    xSemaphoreGive(g_xSemaphoreHandle);
//    GameInit();
    g_xEventCarHandle = xEventGroupCreate();
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
//    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    xTaskCreate(OLED_PrintTask1, "Task1", 128, &g_car[0], osPriorityNormal, NULL);
    xTaskCreate(OLED_PrintTask2, "Task2", 128, &g_car[1], osPriorityNormal + 1, NULL);
    xTaskCreate(OLED_PrintTask3, "Task3", 128, &g_car[2], osPriorityNormal + 2, NULL);
//    xTaskCreate(OLED_PrintTask, "Task4", 128, &g_car[3], osPriorityNormal, NULL);

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
        vTaskDelay(120);
        if (car->x == 120) {
            /* 设置事件 */
            xEventGroupSetBits(g_xEventCarHandle, (1<<0));
            vTaskDelete(NULL);
        }
    }
}

void OLED_PrintTask2(void *params) {
    Car *car = params;
    ShowCar(car);
//    xEventGroupWaitBits(g_xEventCarHandle, (1<<0), pdTRUE, pdTRUE, portMAX_DELAY);
    while (1) {
        HideCar(car);
        car->x += 1;
        if (car->x > 128) { car->x = 120; }
        ShowCar(car);
        vTaskDelay(100);
        if (car->x == 120) {
            xEventGroupSetBits(g_xEventCarHandle, (1<<1));
            vTaskDelete(NULL);
        }
    }
}

void OLED_PrintTask3(void *params) {
    Car *car = params;
    ShowCar(car);
    xEventGroupWaitBits(g_xEventCarHandle, (1<<0)|(1<<1), pdTRUE, pdFALSE, portMAX_DELAY);
    while (1) {
        HideCar(car);
        car->x += 1;
        if (car->x > 128) { car->x = 120; }
        ShowCar(car);
//        vTaskDelay(100);
        HAL_Delay(100);
        if (car->x == 120) {
            vTaskDelete(NULL);
        }
    }
}
/* USER CODE END Application */

