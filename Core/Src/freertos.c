/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
extern bool uart_flag;
extern UART_HandleTypeDef huart1;
bool send_queue=false;
uint16_t count=0;
bool begin=false;
/* USER CODE END Variables */
/* Definitions for led1 */
osThreadId_t led1Handle;
const osThreadAttr_t led1_attributes = {
  .name = "led1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for led2 */
osThreadId_t led2Handle;
const osThreadAttr_t led2_attributes = {
  .name = "led2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for mutex_counter */
osMutexId_t mutex_counterHandle;
const osMutexAttr_t mutex_counter_attributes = {
  .name = "mutex_counter"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);

int __io_putchar(int ch){
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,100);
	return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uart_flag=true;
	UNUSED(huart);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_13){
		begin=true;
	}
  UNUSED(GPIO_Pin);
}

void counter(uint8_t task_numb){
	printf("task%d counter begin\n",task_numb);
	for(uint8_t i=0;i<5;i++){
		printf("count:%d\n",++count);
		HAL_Delay(100);
	}
	count=0;
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutex_counter */
  mutex_counterHandle = osMutexNew(&mutex_counter_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (4, sizeof(uint8_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of led1 */
  led1Handle = osThreadNew(StartDefaultTask, NULL, &led1_attributes);

  /* creation of led2 */
  led2Handle = osThreadNew(StartTask02, NULL, &led2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the led1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint32_t pretime=HAL_GetTick();
	osStatus_t mutex_state=osOK;
  /* Infinite loop */
  for(;;)
  {
	if(HAL_GetTick()-pretime>100){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		pretime=HAL_GetTick();
	}
	mutex_state = osMutexAcquire(mutex_counterHandle,osWaitForever);
	if(mutex_state==osOK){
		if(begin){
			counter(1);
		}
		osMutexRelease(mutex_counterHandle);
	}
	osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the led2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
	uint32_t pretime=HAL_GetTick();
	osStatus_t mutex_state=osOK;
  for(;;)
  {
	  if(HAL_GetTick()-pretime>100){
	  	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
	  	  pretime=HAL_GetTick();
	  }
	  mutex_state = osMutexAcquire(mutex_counterHandle,osWaitForever);
	  if(mutex_state==osOK){
			if(begin){
				counter(2);
			}
		  osMutexRelease(mutex_counterHandle);
	  }
	  osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

