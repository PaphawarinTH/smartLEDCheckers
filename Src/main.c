/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "CheckersGame.h"
#include "setupImage.h"
#include "setupComplete.h"
#include "gameStart.h"
#include "gameOver.h"
#include "Ready.h"
#include "Turn1.h"
#include "Turn2.h"
#include "player1Win.h"
#include "player2Win.h"

#include "stdio.h"
#include "string.h"
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

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint32_t count;
uint32_t count2;
uint32_t count3;
uint32_t input;
uint32_t output;
uint32_t Clear = 0x80000000;
uint32_t canMove = 0x80000000;
uint32_t canMovetoEat = 0x80000000;

uint32_t errorInput;

int setBoard[18];
int Board[18]=
{
	1,1,1,
	1,1,1,
	0,0,0,
	0,0,0,
	3,3,3,
	3,3,3
};
int lift;
int turn = 0;
int end = 0;
char getMove;

int canEat[4] = {0,0,0,0};
int Eaten[4] = {-1,-1,-1,-1};
int time;
int move;
int to;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//void resetGame(uint16_t);
void setLED(int, int);
int cUp_Right(int);
int cUp_Left(int);
int cDown_Right(int);
int cDown_Left(int);
void checkCanMove(int);

void LCD(void);
void delay(int);
int readInput(int);
char InputH(uint32_t);
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
  

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_RNG_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	char str[50];
	/*sprintf(str," ----- Checker Board LED -----\n\r");
	HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);*/
	
	output = Clear + 0x03F03F;
//	input = Clear + 0x000000;
	canMove = Clear;
  canMovetoEat = Clear;
	
	
	int cMove = 1;
	move = -1;
  to = -1;
	
	ILI9341_Init();//initial driver setup to drive ili9341
	
	HAL_ADC_Start(&hadc1);
	
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim3);
	
	uint32_t bInput = input;
	//For LCD
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/*sprintf(str,"Board\n\r"); //For Test
		for(int i=17;i>0;i-=6)
		{
			sprintf(str, "%s%d 0 %d 0 %d 0\n\r", str, readInput(i),readInput(i-1),readInput(i-2));
			sprintf(str, "%s0 %d 0 %d 0 %d\n\r", str, readInput(i-3),readInput(i-4),readInput(i-5));
		}
		HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);
		HAL_Delay(500);*/
		
		if(end>0){}
		else if(count2<5000)
		{
			if(count2>4900) output = Clear + 0x03F03F;
		}
		else if(output == Clear + 0x03F03F)
		{
				while(input!=output)
				{
					if(count2%500==0)
					{
						char str2[50] = "";
						char str3[50] = "";
						uint32_t a=input;
						/*while(a>0)
						{
							sprintf(str2,"%s%c", str2, InputH(a%16));
							a=a/16;
						}
						for(int i=strlen(str2)-1; i>=0;i--){sprintf(str3,"%s%c", str3, str2[i]);}
						sprintf(str3,"%s\n\r", str3);
						HAL_UART_Transmit(&huart3, (uint8_t*)str3, strlen(str3), 1000);*/
						
						if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
							break;
					}
				}
			cMove = 1;
			bInput = input;
			output = Clear;
			for(int i=0; i<18;i++){setBoard[i]=Board[i];}
			
			/*sprintf(str,"Board\n\r");
				for(int i=17;i>0;i-=6)
				{
					sprintf(str, "%s%d 0 %d 0 %d 0\n\r", str, setBoard[i],setBoard[i-1],setBoard[i-2]);
					sprintf(str, "%s0 %d 0 %d 0 %d\n\r", str, setBoard[i-3],setBoard[i-4],setBoard[i-5]);
				}
				HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);*/
			output = Clear + 0x000038;
				
			
				
		}
		else if(bInput != input)
		{
			
			/* USER CODE END WHILE */

			/* USER CODE BEGIN 3 */
			/*if(cMove)
				sprintf(str,"move : ");
			else
				sprintf(str,"move %d to : ", move);
			
			HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);*/
			
			int numInput=0;
			while(1)
			{
				if((bInput&(0x01<<numInput))!=(input&(0x01<<numInput)))
				{
					if(cMove)
					{
						if(!(input&(0x01<<numInput)))
						{
							if((output&(0x01<<numInput)))
							{
								move = numInput;
								break;
							}
							else
							{
								errorInput = output;
								while(1)
								{
									if(count2%200)
									{
										output = Clear;
										if(input != bInput)
										{
											for(int i=0;i<18;i++)
											{
												if((bInput&(0x01<<i))!=(input&(0x01<<i)))
													output = output + (0x01<<i);
											}
										}
										else
										{
											break;
										}
										if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
												break;
									}
									//HAL_Delay(200);
								}
								output = errorInput;
								numInput=0;
							}
						}
						else
						{
							//error
							errorInput = output;
							while(1)
							{
								if(count2%200)
								{
									output = Clear;
									if(input != bInput)
									{
										for(int i=0;i<18;i++)
										{
											if((bInput&(0x01<<i))!=(input&(0x01<<i)))
												output = output + (0x01<<i);
										}
									}
									else
									{
										break;
									}
									if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
											break;
								}
								//HAL_Delay(200);
							}
							output = errorInput;
							numInput=0;
						}
					}
					else
					{
						if((input&(0x01<<numInput)))
						{
							if((output&(0x01<<numInput)))
							{
								to = numInput;
								break;
							}
							else
							{
								errorInput = output;
								while(1)
								{
									if(count2%200==0){
										output = Clear;
										if(input != bInput)
										{
											for(int i=0;i<18;i++)
											{
												if((bInput&(0x01<<i))!=(input&(0x01<<i)))
													output = output + (0x01<<i);
											}
										}
										else
										{
											break;
										}
										if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
												break;
									}
									//HAL_Delay(200);
								}
								output = errorInput;
								numInput=0;
							}
						}
						else
						{
							//moving error
							errorInput = output;
							while(1)
							{
								if(count2%200)
								{
									output = Clear;
									if(input != bInput)
									{
										for(int i=0;i<18;i++)
										{
											if((bInput&(0x01<<i))!=(input&(0x01<<i)))
												output = output + (0x01<<i);
										}
									}
									else
									{
										break;
									}
									if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
											break;
								}
								//HAL_Delay(200);
							}
							output = errorInput;
							numInput=0;
						}
					}
				}
				numInput++;
				if(numInput>17) numInput=0;
				if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)) break;
			}
			
			char str2[50] = "";
			char str3[50] = "";
			uint32_t a=input;
			while(a>0)
			{
				sprintf(str2,"%s%c", str2, InputH(a%16));
				a=a/16;
			}
			for(int i=strlen(str2)-1; i>=0;i--){sprintf(str3,"%s%c", str3, str2[i]);}
			
			/*if(cMove)
			{
				sprintf(str,"(%d)\n\rInput: %s\n\r", move, str3);
			}
			else
			{
				sprintf(str,"(%d)\n\rInput: %s\n\r", to, str3);
			}
			HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);*/
			
			if(cMove)//lift pawn/checker
			{
				lift = setBoard[move];
				for(int i=0;i<4;i++) canEat[i] = 0;
				for(int i=0;i<4;i++) Eaten[i] = -1;
				
				if((output>>move)&0x01)
				{
					if(lift == 1 && turn == 0)
					{
						output = Clear;
						output = output + (0x01<<move);
						cDown_Right(move);
						cDown_Left(move);
						if(canEat[0]>0 || canEat[1]>0 || canEat[2]>0 || canEat[3]>0)
						{
							output = Clear;
							output = output + (0x01<<move);
							if(canEat[0]>0) cUp_Right(canEat[0]);
							if(canEat[1]>0) cUp_Left(canEat[1]);
							if(canEat[2]>0) cDown_Right(canEat[2]);
							if(canEat[3]>0) cDown_Left(canEat[3]);
						}
						setBoard[move] = 0;
						cMove = 0;
					}
					else if(lift == 3 && turn == 1)
					{
						output = Clear;
						output = output + (0x01<<move);
						cUp_Right(move);
						cUp_Left(move);
						if(canEat[0]>0 || canEat[1]>0 || canEat[2]>0 || canEat[3]>0)
						{
							output = Clear;
							output = output + (0x01<<move);
							if(canEat[0]>0) cUp_Right(canEat[0]);
							if(canEat[1]>0) cUp_Left(canEat[1]);
							if(canEat[2]>0) cDown_Right(canEat[2]);
							if(canEat[3]>0) cDown_Left(canEat[3]);
						}
						setBoard[move] = 0;
						cMove = 0;
					}
					else if(lift == 2 && turn == 0)
					{
						output = Clear;
						output = output + (0x01<<move);
						for(int i=move; i!=100; i=cUp_Right(i));
						for(int i=move; i!=100; i=cUp_Left(i));
						for(int i=move; i!=100; i=cDown_Right(i));
						for(int i=move; i!=100; i=cDown_Left(i));
						if(canEat[0]>0 || canEat[1]>0 || canEat[2]>0 || canEat[3]>0)
						{
							output = Clear;
							output = output + (0x01<<move);
							if(canEat[0]>0) cUp_Right(canEat[0]);
							if(canEat[1]>0) cUp_Left(canEat[1]);
							if(canEat[2]>0) cDown_Right(canEat[2]);
							if(canEat[3]>0) cDown_Left(canEat[3]);
						}
						setBoard[move] = 0;
						cMove = 0;
					}
					else if(lift == 4 && turn == 1)
					{
						output = Clear;
						output = output + (0x01<<move);
						for(int i=move; i!=100; i=cUp_Right(i));
						for(int i=move; i!=100; i=cUp_Left(i));
						for(int i=move; i!=100; i=cDown_Right(i));
						for(int i=move; i!=100; i=cDown_Left(i));
						if(canEat[0]>0 || canEat[1]>0 || canEat[2]>0 || canEat[3]>0)
						{
							output = Clear;
							output = output + (0x01<<move);
							if(canEat[0]>0) cUp_Right(canEat[0]);
							if(canEat[1]>0) cUp_Left(canEat[1]);
							if(canEat[2]>0) cDown_Right(canEat[2]);
							if(canEat[3]>0) cDown_Left(canEat[3]);
						}
						setBoard[move] = 0;
						cMove = 0;
					}
				}
			}
			else//place pawn/checker
			{
				if((output>>to)&0x01)
				{
					int nextTurn=1;
					setBoard[to] = lift;
					canMove = Clear;
					canMovetoEat = Clear;
					if(to != move)
					{
						for(int i=0;i<4;i++)
						{
							if(to == Eaten[i])
							{
								setBoard[canEat[i]]=0;
								bInput = bInput - (0x01<<canEat[i]) + (0x01<<Eaten[i]);
								while(1)//REMOVE Eaten
								{
									if(count%200==0)
									{
										output = Clear;
										if(bInput != input)
										{
											for(int j=0;j<18;j++)
											{
												if((bInput&(0x01<<j))!=(input&(0x01<<j)))
													output = output + (0x01<<j);
												if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
													break;
											}
										}
										else
											break;
									}
									//HAL_Delay(200);
								}
								for(int j=0;j<4;j++) Eaten[j] = -1;
								checkCanMove(to);
								for(int j=0;j<4;j++) if(Eaten[j]+1>0) nextTurn=0;
							}
						}
						if(to<3&&lift==3)
						{
							setBoard[to]=4;
							nextTurn=1;
						}
						else if(to>14&&lift==1)
						{
							setBoard[to]=2;
							nextTurn=1;
						}
						
						if(nextTurn)
						{
							if(turn) turn = 0;
							else turn = 1;
						}
					}
					
					canMove = Clear;
					/* check pawn or checker which can move */
					canMovetoEat = Clear;
					for(int i=0;i<18;i++)
					{
						checkCanMove(i);
					}
					
					
					if(canMovetoEat>Clear)
						output = canMovetoEat;
					else
						output = canMove;
					cMove = 1;
				}
			}
			/*sprintf(str,"Board\n\r");
			for(int i=17;i>0;i-=6)
			{
				sprintf(str, "%s%d 0 %d 0 %d 0\n\r", str, setBoard[i],setBoard[i-1],setBoard[i-2]);
				sprintf(str, "%s0 %d 0 %d 0 %d\n\r", str, setBoard[i-3],setBoard[i-4],setBoard[i-5]);
			}
			HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);*/
			
			int has1 = 0;
			int has2 = 0;
			for(int i=0;i<18;i++)
			{
				if(setBoard[i]==1||setBoard[i]==2||lift==1||lift==2) has1=1;
				if(setBoard[i]==3||setBoard[i]==4||lift==3||lift==4) has2=1;
			}
			if(!has1 || !has2)
			{	
				if(has1) end = 1;
				else if(has2) end = 2;
			}
			bInput = input;
		}
		else
		{
			if(output==Clear)
				end = 3;
		}
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int cUp_Right(int move)
{
	if(move>2)
	{
		if((move/3)%2)
		{
			if(!(((move-2)/3)%2))
			{
				if(setBoard[move-2]==0)
				{
					output = output + (0x01<<(move-2));
					return move-2;
				}
				else 
				{
					if((setBoard[move-2]<3 && lift > 2) || (setBoard[move-2]>2 && lift == 2))
					{
						if(setBoard[move-2-3]==0 && (((move-2-3)/3)%2) && move-2-3>=0)
						{
							canEat[0] = move-2;
							Eaten[0] = move-2-3;
							output = output + (0x01<<(move-2-3));
							return 99;
						}
					}
				}
			}
		}
		else
		{
			if(setBoard[move-3]==0)
			{
				output = output + (0x01<<(move-3));
				return move-3;
			}
			else 
			{
				if((setBoard[move-3]<3 && lift > 2) || (setBoard[move-3]>2 && lift == 2))
				{
					if(setBoard[move-3-2]==0 && !(((move-3-2)/3)%2) && move-3-2>=0)
					{
						canEat[0] = move-3;
						Eaten[0] = move-3-2;
						output = output + (0x01<<(move-3-2));
						return 99;
					}
				}
			}
		}
	}
	return 100;
}

int cUp_Left(int move)
{
	if(move>2)
	{
		if((move/3)%2)
		{
			if(setBoard[move-3]==0)
			{
				output = output + (0x01<<(move-3));
				return move-3;
			}
			else 
			{
				if((setBoard[move-3]<3 && lift > 2) || (setBoard[move-3]>2 && lift == 2))
				{
					if(setBoard[move-3-4]==0 && (((move-3-4)/3)%2) && move-3-4>=0)
					{
						canEat[1] = move-3;
						Eaten[1] = move-3-4;
						output = output + (0x01<<(move-3-4));
						return 99;
					}
				}
			}
		}
		else
		{
			if(((move-4)/3)%2)
			{
				if(setBoard[move-4]==0)
				{
					output = output + (0x01<<(move-4));
					return move-4;
				}
				else 
				{
					if((setBoard[move-4]<3 && lift > 2) || (setBoard[move-4]>2 && lift == 2))
					{
						if(setBoard[move-4-3]==0 && !(((move-4-3)/3)%2) && move-4-3>=0)
						{
							canEat[1] = move-4;
							Eaten[1] = move-4-3;
							output = output + (0x01<<(move-4-3));
							return 99;
						}
					}
				}
			}
		}
	}
	return 100;
}

int cDown_Right(int move)
{
	if(move<15)
	{
		if((move/3)%2)
		{
			if(!(((move+4)/3)%2))
			{
				if(setBoard[move+4]==0)
				{
					output = output + (0x01<<(move+4));
					return move+4;
				}
				else 
				{
					if((setBoard[move+4]>2 && lift < 3) || (setBoard[move+4]<3 && lift == 4))
					{
						if(setBoard[move+4+3]==0 && (((move+4+3)/3)%2) && move+4+3<18)
						{
							canEat[2] = move+4;
							Eaten[2] = move+4+3;
							output = output + (0x01<<(move+4+3));
							return 99;
						}
					}
				}
			}
		}
		else
		{
			if(setBoard[move+3]==0)
			{
				output = output + (0x01<<(move+3));
				return move+3;
			}
			else 
			{
				if((setBoard[move+3]>2 && lift < 3) || (setBoard[move+3]<3 && lift == 4))
				{
					if(setBoard[move+3+4]==0 && !(((move+3+4)/3)%2) && move+3+4<18)
					{
						canEat[2] = move+3;
						Eaten[2] = move+3+4;
						output = output + (0x01<<(move+3+4));
						return 99;
					}
				}
			}
		}
	}
	return 100;
}

int cDown_Left(int move)
{
	if(move<15)
	{
		if((move/3)%2)
		{
			if(setBoard[move+3]==0)
			{
				output = output + (0x01<<(move+3));
				return move+3;
			}
			else 
			{
				if((setBoard[move+3]>2 && lift < 3) || (setBoard[move+3]<3 && lift == 4))
				{
					if(setBoard[move+3+2]==0 && (((move+3+2)/3)%2) && move+3+2<18)
					{
						canEat[3] = move+3;
						Eaten[3] = move+3+2;
						output = output + (0x01<<(move+3+2));
						return 99;
					}
				}
			}
		}
		else
		{
			if(((move+2)/3)%2)
			{
				if(setBoard[move+2]==0)
				{
					output = output + (0x01<<(move+2));
					return move+2;
				}
				else 
				{
					if((setBoard[move+2]>2 && lift < 3) || (setBoard[move+2]<3 && lift == 4))
					{
						if(setBoard[move+2+3]==0 && !(((move+2+3)/3)%2) && move+2+3<18)
						{
							canEat[3] = move+2;
							Eaten[3] = move+2+3;
							output = output + (0x01<<(move+2+3));
							return 99;
						}
					}
				}
			}
		}
	}
	return 100;
}

void checkCanMove(int i)
{
	lift = setBoard[i];
	if(lift==1 && turn == 0)
	{
		if(cDown_Right(i)!=100||cDown_Left(i)!=100)
			canMove += 0x01<<i;
		if(cDown_Right(i)==99||cDown_Left(i)==99)
			canMovetoEat |= 0x01<<i;
	}
	else if(lift==3 && turn == 1)
	{
		if(cUp_Right(i)!=100||cUp_Left(i)!=100)
			canMove += 0x01<<i;
		if(cUp_Right(i)==99||cUp_Left(i)==99)
			canMovetoEat |= 0x01<<i;
	}
	else if(lift==2 && turn == 0)
	{
		if(cDown_Right(i)||cDown_Left(i)||cUp_Right(i)||cUp_Left(i))
			canMove += 0x01<<i;
			for(int j=i; j!=100; j=cUp_Right(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cUp_Left(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cDown_Right(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cDown_Left(j)) if(j==99) canMovetoEat |= 0x01<<i;
	}
	else if(lift==4  && turn == 1)
	{
		if(cDown_Right(i)||cDown_Left(i)||cUp_Right(i)||cUp_Left(i))
			canMove += 0x01<<i;
			for(int j=i; j!=100; j=cUp_Right(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cUp_Left(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cDown_Right(j)) if(j==99) canMovetoEat |= 0x01<<i;
			for(int j=i; j!=100; j=cDown_Left(j)) if(j==99) canMovetoEat |= 0x01<<i;
	}
}



/* ---- LCD CONTROL ---- */
void LCD()
{	
	if(count2<2000)
	{
		output = Clear + 0x0FFFFF;
		ILI9341_Draw_Image((const char*)checkersGame, SCREEN_VERTICAL_2);		
		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
	}
	else if(count2 < 5000)
	{
		output = Clear + (0x07<<(((count2/100)%6)*3));
		ILI9341_Draw_Image((const char*)gameStart, SCREEN_VERTICAL_2);		
		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		if(count2>4900)
			output = Clear + 0x03F03F;
	}
	else if(output==Clear+0x03F03F)
	{
		if(input != output)
		{
			ILI9341_Draw_Image((const char*)setupImage, SCREEN_VERTICAL_2);		
			ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
			time = count2;
		}
	}
	else
	{
		if(count2-time < 1800)
		{
			ILI9341_Draw_Image((const char*)setupComplete, SCREEN_VERTICAL_2);		
			ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		}
		else if(count2-time < 3600)
		{
			ILI9341_Draw_Image((const char*)ready, SCREEN_VERTICAL_2);		
			ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
		}
		
		else
		{
			if(end)
			{
				if(end==1)
				{
					ILI9341_Draw_Image((const char*)player1Win, SCREEN_VERTICAL_2);		
					ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
				}
				else if(end==2)
				{
					ILI9341_Draw_Image((const char*)player2Win, SCREEN_VERTICAL_2);		
					ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
				}
				else if(end==3)
				{
					ILI9341_Draw_Image((const char*)gameOver, SCREEN_VERTICAL_2);		
					ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
				}
				
			}
			else if(turn==0)
			{
				ILI9341_Draw_Image((const char*)turn1, SCREEN_VERTICAL_2);		
				ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
			}
			else
			{
				ILI9341_Draw_Image((const char*)turn2, SCREEN_VERTICAL_2);		
				ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
			}
		}
		
	}
}

char InputH(uint32_t inputH)
{
	switch(inputH)
	{
		case 0: return '0';
			break;
		case 1: return '1';
			break;
		case 2: return '2';
			break;
		case 3: return '3';
			break;
		case 4: return '4';
			break;
		case 5: return '5';
			break;
		case 6: return '6';
			break;
		case 7: return '7';
			break;
		case 8: return '8';
			break;
		case 9: return '9';
			break;
		case 10: return 'A';
			break;
		case 11: return 'B';
			break;
		case 12: return 'C';
			break;
		case 13: return 'D';
			break;
		case 14: return 'E';
			break;
		case 15: return 'F';
			break;
	}
}
	
int readInput(int nInput)
{
	switch(nInput)
	{
		case 0: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
			break;
		case 1: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
			break;
		case 2: return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
			break;
		case 3: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
			break;
		case 4: return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
			break;
		case 5: return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
			break;
		case 6: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
			break;
		case 7: return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);
			break;
		case 8: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
			break;
		case 9: return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
			break;
		case 10: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
			break;
		case 11: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
			break;
		case 12: return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14);
			break;
		case 13: return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
			break;
		case 14: return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
			break;
		case 15: return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
			break;
		case 16: return HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_12);
			break;
		case 17: return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15);
			break;
	}
	
}

/* ---- RESET GAME INTERRUPT ---- */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_12)
	{
		count = 0;
		count2 = 0;
		turn = 0;
		end = 0;
		output = Clear + 0x03F03F;
		for(int i=0; i<18;i++){setBoard[i]=Board[i];}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
