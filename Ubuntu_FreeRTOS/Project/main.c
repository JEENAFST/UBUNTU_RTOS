
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void SystemClock_Config(void);

SemaphoreHandle_t xMutex;
TaskHandle_t Enty_Task_Handle =NULL;
TaskHandle_t Exit_Task_Handle =NULL;
/* Private function prototypes -----------------------------------------------*/

static void vEntry_Task(void *pvparameters);
static void vExit_Task(void *pvparameters);

/* USER CODE BEGIN PFP */

int g_count;

int main(void)
{

  SystemClock_Config();

  xTaskCreate(vEntry_Task,"Entry counter",200,(void*)g_count,1,&Enty_Task_Handle);
  xTaskCreate(vExit_Task,"Exit counter",200,(void*)g_count,1,&Exit_Task_Handle);
  xMutex=xSemaphoreCreateMutex();
  vTaskStartScheduler();
  while (1)
  {

  }

}


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */

/* USER CODE BEGIN 4 */
 static void vEntry_Task(void *p)
 {
	while(1){
	if(xSemaphoreTake(xMutex,portMAX_DELAY)==1)//mutex lock
		{
			//Task1


       if (g_count>30)
       {
    	   xSemaphoreGive(xMutex);
    	   vTaskDelete(Enty_Task_Handle);

       }
       g_count=g_count+1;

       vTaskDelay(500);//1
       xSemaphoreGive(xMutex);


		}
       }
}
static void vExit_Task(void *p){
	while(1)
	{
		if(xSemaphoreTake(xMutex,portMAX_DELAY)==1)

		{
			//Task2


		       if (g_count<1)
		       {
		    	   xSemaphoreGive(xMutex);
		    	   vTaskDelete(Exit_Task_Handle);

		       }
                  g_count=g_count-1;

			}
		 xSemaphoreGive(xMutex);
		 vTaskDelay(500);

	}
		vTaskDelay(100);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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



