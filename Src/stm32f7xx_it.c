/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
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
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "usart.h"
#include "tim.h"
#include "adc.h"

void setLED(int, int);
void resetLED(void);
extern int readInput(int);
extern void LCD(void);

extern uint32_t count;
extern uint32_t count2;
extern uint32_t input;
extern uint32_t output;
extern uint32_t Clear;


char Start = 250;
char ProgramChange = 192;
char ProgramChannel = 0;

uint32_t inputSwitch = 0;
uint32_t adc_val;
float level;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
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

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
	if(count>3000)
	{
		HAL_ADC_Start(&hadc1);
		adc_val = HAL_ADC_GetValue(&hadc1);
		ProgramChannel = adc_val*128/4096;
		HAL_UART_Transmit(&huart3,(uint8_t*)&Start,1,1000);
		HAL_UART_Transmit(&huart3,(uint8_t*)&ProgramChange,1,1000);
		HAL_UART_Transmit(&huart3,(uint8_t*)&ProgramChannel,1,1000);
	}
	
	if(count%18==0)
	{
		inputSwitch = Clear;
	}
	if(readInput(count%18))
		inputSwitch = inputSwitch + (0x01<<(count%18));
	
	if(count%18==17)
	{
		input = inputSwitch;
	}
  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */
	
	/*Output for LED*/
	resetLED();
	uint32_t outLED = output>>(3*(count%6));
	setLED(0, !(outLED & 0x01));
	setLED(1, !((outLED>>1) & 0x01));
	setLED(2, !((outLED>>2) & 0x01));
	
	//For Test
	/*setLED(0, !readInput((3*(count%6))));
	setLED(1, !readInput((3*(count%6)+1)));
	setLED(2, !readInput((3*(count%6)+2)));*/
	
	count++;
  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	count2++;
	
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	if(count2%500==0) LCD();
  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void resetLED()
{
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
	
	switch(count%6)
	{
		case 0: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
			break;
		case 1: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET);
			break;
		case 2: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);
			break;
		case 3: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);
			break;
		case 4: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET);
			break;
		case 5: HAL_GPIO_WritePin(GPIOF,GPIO_PIN_15,GPIO_PIN_SET);
			break;
	}
	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,GPIO_PIN_SET);
}
void setLED(int nLED, int set)
{
	switch(nLED)
	{
		case 0: HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,set);
			break;
		case 1: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,set);
			break;
		case 2: HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7,set);
			break;
	}
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
