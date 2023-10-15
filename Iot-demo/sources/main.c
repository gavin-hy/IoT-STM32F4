#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>

#include "led.h"
#include "usart.h"


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "elog.h"

#define LOG_TAG "main"

void SystemClock_Config(void);
void Error_Handler(void);


void Log_Tesk(void *pt)
{
    elog_init();
    elog_set_fmt(ELOG_LVL_ERROR,OUT_LOG_TABLE);
    elog_set_fmt(ELOG_LVL_WARN, OUT_LOG_TABLE);
    elog_set_fmt(ELOG_LVL_INFO, OUT_LOG_TABLE);
    elog_start();
    while(1){
//        log_e("Hello EasyLogger!");
//        log_w("Hello EasyLogger!");
        log_i("EasyLogger Running!");
         //printf("EasyLogger Running!\r\n");
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}
void Led_Task(void* pt)
{
    led_init();
    while(1){
        HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
        //printf("LED Task Running!\r\n");
        vTaskDelay(200/portTICK_PERIOD_MS);
        log_i("LED Task Running!!!");
         
    }
}

int main(void)
{
    uint8_t timebase;
    HAL_Init();
    
    SystemClock_Config();
    
    led_init();
    usart_init();
    printf("Hello!\r\n");
          
    if(pdPASS== xTaskCreate(Led_Task,"Led_Task",1000,NULL,3,NULL))
    {
        printf("Create Success!");
    }
   if(pdPASS== xTaskCreate(Log_Tesk,"Log_Tesk",1000,NULL,2,NULL))
    {
        printf("Create Success!");
    }
    vTaskStartScheduler();
 
   return 0;
}



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
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}