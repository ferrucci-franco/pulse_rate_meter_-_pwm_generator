/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mb.h"
#include "mbport.h"
#include "mbtask.h"
#include "pulses_input_and_pwm_output.h"
#include "MAX7219.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint32_t window_is_open = 0;
uint32_t pulse_cnt[PULSE_NUMBER_OF_INPUTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t pulse_last_cnt[PULSE_NUMBER_OF_INPUTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
//uint8_t uart_tx_header[] = {0xFF, 0xFF, 0xFF, 0xFF};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void EXTI_Callback();
void SysTick_Callback();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
extern uint16_t downcounter;

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
    SysTick_Callback();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI0_IRQn 0 */
  /* USER CODE BEGIN EXTI0_IRQn 1 */

  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI1_IRQn 0 */
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
		EXTI_Callback();
  /* USER CODE END EXTI2_IRQn 0 */
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI3_IRQn 0 */
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI4_IRQn 0 */
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI9_5_IRQn 0 */
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
    static uint16_t frame_cnt = 0;

    /* Close counters window */
    window_is_open = 0;

	/* Transfer results */
	for (uint32_t i = 0; i < PULSE_NUMBER_OF_INPUTS; i++)
	{
		pulse_last_cnt[i] = pulse_cnt[i];
		mb_input_register_buffer[2*i] = (uint16_t)(pulse_cnt[i] << 16);
		mb_input_register_buffer[2*i+1] = (uint16_t)(pulse_cnt[i] & 0x0000FFFF);
		pulse_cnt[i] = 0U; // reset counters
	}
    
    /* Increment frame counter and transfer it to Modbus buffer */
    frame_cnt += 1;
    mb_input_register_buffer[MB_REG_INPUT_FRAME] = frame_cnt;
    
    /* Toggle "is alive" LEDs */
	HAL_GPIO_TogglePin(LED_FRONT_GPIO_Port, LED_FRONT_Pin);
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin); 

    ///* Send data through serial line */
	//HAL_UART_Transmit(&huart2, uart_tx_header, 4, 1U); // header
	//HAL_UART_Transmit(&huart2, (uint8_t*) pulse_last_cnt, 4*PULSE_NUMBER_OF_INPUTS, 1U); // data

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

    /* Reopen counters window */
	window_is_open = 1;

    /* Re-start timer (since it is configure as "one-pulse") */
    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1); 

    /* Update display */
    MAX7219_DisplayNibbleInt(0, pulse_last_cnt[2]);
    MAX7219_DisplayNibbleInt(1, pulse_last_cnt[1]);
    
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE);
	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE);

	if((tmp_flag != RESET) && (tmp_it_source != RESET))
	{
	  pxMBFrameCBByteReceived();
	  __HAL_UART_CLEAR_PEFLAG(&huart2);    
	  return;
	}

	if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET) &&(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TXE) != RESET))
	{
	  pxMBFrameCBTransmitterEmpty();    
	  return ;
	}
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	EXTI_Callback();
  /* USER CODE END EXTI15_10_IRQn 0 */
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	if(__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE) != RESET && __HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) !=RESET)
	{
		__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
		if (!--downcounter)
		{
    	pxMBPortCBTimerExpired();
		}
  }
  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void EXTI_Callback()
{
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
	{
		if(window_is_open)
		{
			pulse_cnt[0] += 1;	// blue push button
			HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		}
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[2] += 1;	// PULSE_01 pin mapped to FRONT input number 2
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[12] += 1; // PULSE_02 pin mapped to FRONT input number 12
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[7] += 1;	// PULSE_03 pin mapped to FRONT input number 7
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[8] += 1;	// PULSE_04 pin mapped to FRONT input number 8
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[3] += 1;	// PULSE_05 pin mapped to FRONT input number 3
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[6] += 1;	// PULSE_06 pin mapped to FRONT input number 6
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[1] += 1;	// PULSE_07 pin mapped to FRONT input number 1
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
	}
	
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[9] += 1;	// PULSE_08 pin mapped to FRONT input number 9
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
	}

	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET)
	{
		if(window_is_open)
		{
            pulse_cnt[10] += 1;	// PULSE_09 pin mapped to FRONT input number 10
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
	}

	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET)
	{
		if(window_is_open)
		{
            pulse_cnt[5] += 1;	// PULSE_10 pin mapped to FRONT input number 5
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
	}

	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET)
	{
		if(window_is_open)
		{		
            pulse_cnt[11] += 1;	// PULSE_11 pin mapped to FRONT input number 11
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
	}

	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
	{
		if(window_is_open)
		{
            pulse_cnt[4] += 1;	// PULSE_12 pin mapped to FRONT input number 4
            HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
        }
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
	}	
}


void SysTick_Callback()
{
	/* Modbus polling */
	eMBPoll();

	/* Process Modbus PWM commands */
    static uint32_t is_first_time_entering_in_SysTick_Callback = 1;
    
    if (is_first_time_entering_in_SysTick_Callback || Modbus_RegHolding_have_changed())
    {
        /* Turn off flag */
        is_first_time_entering_in_SysTick_Callback = 0;
        
        /* Update PWM timers */
        if (PWM_Error == MB_PWM_Update())
        {
            HAL_GPIO_TogglePin(LD6_GPIO_Port, LD6_Pin);
        }
        /* Update display PWM 1*/
        if (mb_holding_register_buffer[PWM1_STATUS] == PWM_DISABLE)
            MAX7219_DisplayNibbleDutyCycle(3, 0);
        else
            MAX7219_DisplayNibbleDutyCycle(3, mb_holding_register_buffer[PWM1_DUTY_CYCLE]);

        /* Update display PWM 2*/
        if (mb_holding_register_buffer[PWM2_STATUS] == PWM_DISABLE)
            MAX7219_DisplayNibbleDutyCycle(2, 0);
        else
            MAX7219_DisplayNibbleDutyCycle(2, mb_holding_register_buffer[PWM2_DUTY_CYCLE]);
    }
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
