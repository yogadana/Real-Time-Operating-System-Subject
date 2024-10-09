/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
int x_val, button1_pressed;
uint8_t choice;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for pickButtonTask */
osThreadId_t pickButtonTaskHandle;
const osThreadAttr_t pickButtonTask_attributes = {
  .name = "pickButtonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED_LVL_task */
osThreadId_t LED_LVL_taskHandle;
const osThreadAttr_t LED_LVL_task_attributes = {
  .name = "LED_LVL_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for getADCtask */
osThreadId_t getADCtaskHandle;
const osThreadAttr_t getADCtask_attributes = {
  .name = "getADCtask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for dispUARTTask */
osThreadId_t dispUARTTaskHandle;
const osThreadAttr_t dispUARTTask_attributes = {
  .name = "dispUARTTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void *argument);
void pickButton(void *argument);
void LED_LVL(void *argument);
void getADC(void *argument);
void dispUART(void *argument);

/* USER CODE BEGIN PFP */

void Light_LED(){
	if(x_val < 4096/5){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
		} else if(x_val < 4096*2/5){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
		} else if(x_val < 4096*3/5){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
		} else if(x_val < 4096*4/5){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
		} else{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1);
		}
}

void Tf_LED(){
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
}


void Menu_Display(){
	char buffer[20] = "_";
	sprintf(buffer, "Voltage = %d\r\n",x_val);
	HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), 20);
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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

  /* creation of pickButtonTask */
  pickButtonTaskHandle = osThreadNew(pickButton, NULL, &pickButtonTask_attributes);

  /* creation of LED_LVL_task */
  LED_LVL_taskHandle = osThreadNew(LED_LVL, NULL, &LED_LVL_task_attributes);

  /* creation of getADCtask */
  getADCtaskHandle = osThreadNew(getADC, NULL, &getADCtask_attributes);

  /* creation of dispUARTTask */
  dispUARTTaskHandle = osThreadNew(dispUART, NULL, &dispUARTTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED3_Pin|LED4_Pin|LED5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED3_Pin LED4_Pin LED5_Pin */
  GPIO_InitStruct.Pin = LED3_Pin|LED4_Pin|LED5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_pickButton */
/**
* @brief Function implementing the pickButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pickButton */
void pickButton(void *argument)
{
  /* USER CODE BEGIN pickButton */
  /* Infinite loop */
  for(;;)
  {

	  if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_RESET)
	  {
		  osDelay(300);	// debounce
		  button1_pressed = 1;
	  }
	  else button1_pressed = 0;

//	  if(HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == GPIO_PIN_RESET)
//	  {
//		  osDelay(300);	// debounce
//		  button2_pressed = 1;
//	  }

    osDelay(100);
  }
  /* USER CODE END pickButton */
}

/* USER CODE BEGIN Header_LED_LVL */
/**
* @brief Function implementing the LED_LVL_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_LVL */
void LED_LVL(void *argument)
{
  /* USER CODE BEGIN LED_LVL */
  /* Infinite loop */
	while(1)
  {
	  if(button1_pressed == 1){
			Light_LED();
			osDelay(300);
			Tf_LED();
			osDelay(300);
		}
		else Light_LED();
  }
  /* USER CODE END LED_LVL */
}

/* USER CODE BEGIN Header_getADC */
/**
* @brief Function implementing the getADCtask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_getADC */
void getADC(void *argument)
{
  /* USER CODE BEGIN getADC */
  /* Infinite loop */
  while(1)
  {

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  x_val = (HAL_ADC_GetValue(&hadc1));
	  HAL_ADC_Stop(&hadc1);

	  // process ADC values here


    osDelay(10);
  }
  /* USER CODE END getADC */
}

/* USER CODE BEGIN Header_dispUART */
/**
* @brief Function implementing the dispUARTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_dispUART */
void dispUART(void *argument)
{
  /* USER CODE BEGIN dispUART */
  /* Infinite loop */
	Menu_Display();
	char buffer2[20] = "_";

	  for(;;)
	  {
		  // select user input
		  if (HAL_UART_Receive(&huart2, &choice, sizeof(choice), 10) == HAL_OK)
		  {
			switch (choice) {

				case '1':
					//HAL_UART_Transmit(&huart1, (uint8_t*)"2 pressed\r\n", sizeof("1 pressed\r\n"), 10);
					sprintf(buffer2, "Voltage = %d\r\n",x_val);
					HAL_UART_Transmit(&huart2, buffer2, sizeof(buffer2), 20);
					break;
				case '2':
					Menu_Display();
					break;
				default:
					break;
			}
		  }else{

		  }

		  if(button1_pressed)
		  {
			  HAL_UART_Transmit(&huart2, (uint8_t*)"Button1 pressed\r\n", sizeof("Button1 pressed\r\n"), 10);
//			  button1_pressed = 0;
		  }

	//		  if(button2_pressed)
	//		  {
	//			  HAL_UART_Transmit(&huart1, (uint8_t*)"Button2 pressed\r\n", sizeof("Button2 pressed\r\n"), 10);
	//			  button2_pressed = 0;
	//		  }


		osDelay(100);
	  }
  /* USER CODE END dispUART */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
