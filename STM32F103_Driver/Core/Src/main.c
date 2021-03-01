/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "I2C.h"
#include "TIMER.h"
#include "MOTOR.h"
#include "SPI.h"
#include "USART.h"
#include "GPIO.h"
#include <stdio.h>
/* USER CODE END Includes */
#define Car		1
#define tim3_encoder_read_ok	false
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//#define RXBUFFERSIZE 7
//static const uint8_t STM_ADDR = 0x04; // Use 8-bit address
//uint8_t aRxBuffer[RXBUFFERSIZE];//gia tri nhan truc tiep tu i2c
//uint8_t dataBuffer[RXBUFFERSIZE];//gia tri luu lai qua i2c
//uint32_t ReceiverCount;//bien dem gia tri nhan
char Rx_Data[2];
uint8_t Rx_Index = 0;

#define RXBUFFERSIZE 4
uint8_t aRxBuffer[RXBUFFERSIZE];
volatile int  angle, mode;
double speed;
/* USER CODE END PTD */
//#define Left	1
//#define	Right	2
//#define	Straight	0
//#define Stop	3
#define _speed	220

#define Left	49
#define	Right	50
#define	Straight	48
#define Stop	51


#if (Car == 1)

#define	NumPreRunLeft		730
#define	NumPreRunRight	730
#define AngleLeft				-30
#define AngleRight			43
#define	NumEncoderLeft	1235
#define NumEncoderRight	1170
#endif
#if (Car == 2)
#define	NumPreRunLeft		650
#define	NumPreRunRight	730
#define AngleLeft				-33
#define AngleRight			55
#define	NumEncoderLeft	1235
#define NumEncoderRight	1235
#endif
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/




PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
extern SPI_HandleTypeDef hspi1;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

//__IO int32_t Num_Encoder_1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_USB_PCD_Init(void);

void RunLeft(void);
void RunRight(void);
double pid_Left(double speed_L, double duty_L, float kp_L, float ki_L, float kd_L);
double pid_Right(double speed_R, double duty_R, float kp_R, float ki_R, float kd_R);
uint8_t State= Straight ;

/* USER CODE BEGIN PFP */
	char Rx_data[2];
	int read_encoder1,read_encoder2,flag=0,count_tim;
//#define kp 2     
//#define ki 0.004
//#define kd 1.5
	float kp_L= 4, ki_L=9.00000032e-06,kd_L=1.5;
	float kp_R= 4, ki_R=9.00000032e-06,kd_R=1.5;
	float error=0;//debug xong xoa
//	float kp,ki,kd;
	double i_L,d_L,preError_L=0,precount_L=0,duty_L=0,i_R,d_R,preError_R=0,precount_R=0,duty_R=0;
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
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_Init();
  MX_USB_PCD_Init();
//  MX_SPI1_Init();

//  MX_I2C1_Init();
	
	//printf("Program's starting ...\n\r");


  /* Infinite loop */
	HAL_UART_Receive_IT(&huart2, aRxBuffer, RXBUFFERSIZE);
	HAL_SPI_Receive_DMA(&hspi1,aRxBuffer,RXBUFFERSIZE);
	setAngle(0);
//	LeftMotor(1,300);
//	RightMotor(1,300);

////	HAL_Delay(1000);
////	setAngle(-20);
//	HAL_Delay(1000);
//	LeftMotor(0,300);
//	RightMotor(0,300);
	printf("Start ... %d\t%d\n\r", Encoder1_Get_Counter(),Encoder2_Get_Counter());
	
  while (1){
//		printf("Program's running ... %d\t    %d\n\r", Encoder1_Get_Counter(),Encoder2_Get_Counter());
		switch (State){
			case Left: RunLeft(); break;
			case Right: RunRight(); break;
			case Stop: 
					setAngle(0);
					LeftMotor(0,0);
					RightMotor(0,0);
					break;
			default: break;
				
		}
		//add by dat
		if(flag==1)
		{
			flag=0;
			duty_L=pid_Left(speed,duty_L,kp_L,ki_L,kd_L);
			duty_R=pid_Right(speed,duty_R,kp_R,ki_R,kd_R);
			if(duty_R>=400)
			{
				duty_R=400;
			}else if(duty_R<=0)
			{
				duty_R=0;
			}
			if(duty_L>=400)
			{
				duty_L=400;
			}else if(duty_L<=0)
			{
				duty_L=0;
			}
			Set_Encoder();
		}
		LeftMotor(1, duty_L);
		RightMotor(1, duty_R);
//		read_encoder1= Encoder1_Get_Counter()-32767;
//		read_encoder2= Encoder2_Get_Counter();
		//end
//		HAL_Delay(1000);
		if(!HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin)){
			while(!HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin));
			printf("Left ... %d\t%d\n\r", Encoder1_Get_Counter(),Encoder2_Get_Counter());
			RunLeft();
			
		}
		if(!HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin)){
			while(!HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin));
			RunRight();
			
		}
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_PCD_Init(void)
{
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	uint8_t temp;
//	if(huart->Instance == USART2){
//		temp = Rx_Data[0];
//		HAL_UART_Transmit(&huart2, (uint8_t *)&temp, 1, 10);
//		Rx_Data[0] = 0;
//		HAL_UART_Receive_IT(&huart2, (uint8_t*)&Rx_Data[0], 1);
//  }
//	if (huart->Instance == huart2.Instance)
//	{
//		if (aRxBuffer[RXBUFFERSIZE - 1] == 32)
//		{
////			speed = (int)aRxBuffer[0];    
////			angle = (int)aRxBuffer[1];
////			mode = (int)aRxBuffer[2];
//			//1 vuong trai
//			//2 vuong phai
//			// 0 bth
//			
//			if(mode == Straight){
//				angle = (angle - 50);
//				setAngle(angle);
//				speed = speed * 4;
//				if(angle > 20 ){
//					RightMotor(1, 90);
//					LeftMotor(1, 180);
//				}
//				else if (angle < -20){
//					RightMotor(1, 90);
//					LeftMotor(1, 180);
//				}
//				else{
//					RightMotor(1, 180);
//					LeftMotor(1, 180);
//				}
//			}
//			if (mode == Stop){
//				State = Stop;
//			}
//			if (mode == Right){// cua phai, banh trai dem leen    en1  ab2
//				State = Right;
//			}
//			if  (mode == Left){ // cua trai, banh phai dem xuong  ab1
//				State = Left;
//			}

//			HAL_UART_Transmit_IT(&huart2, aRxBuffer, RXBUFFERSIZE);
//			HAL_UART_Receive_IT(&huart2, aRxBuffer, RXBUFFERSIZE);
//		}
//	}
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	if (hspi->Instance == hspi1.Instance)
//	{
////		int co_dem;
//			if (aRxBuffer[RXBUFFERSIZE - 1] == 32)
//		{
////			speed = (int)aRxBuffer[0];
////			angle = (int)aRxBuffer[1];
////			mode = (int)aRxBuffer[2];
//			//1 vuong trai
//			//2 vuong phai
//			// 0 bth
//			
//			if(mode == Straight){
//				angle = (angle - 50);
//				setAngle(angle);
//				speed = speed * 4;
//				if(angle > 20 ){
//					RightMotor(1, 90);
//					LeftMotor(1, 180);
//				}
//				else if (angle < -20){
//					RightMotor(1, 90);
//					LeftMotor(1, 180);
//				}
//				else{
//					RightMotor(1, 180);
//					LeftMotor(1, 180);
//				}
//			}
//			if (mode == Stop){
//				State = Stop;
//			}
//			if (mode == Right){// cua phai, banh trai dem leen    en1  ab2
//				State = Right;
//			}
//			if  (mode == Left){ // cua trai, banh phai dem xuong  ab1
//				State = Left;
//			}
////			HAL_UART_Transmit_IT(&huart2, aRxBuffer, RXBUFFERSIZE);
////			HAL_UART_Receive_IT(&huart2, aRxBuffer, RXBUFFERSIZE);
//		}
//		
//	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim4.Instance)
	{
		count_tim++;
		if(count_tim>=5)
		{
			flag=1;
			count_tim=0;
		}
	}
}
/* USER CODE BEGIN 4 */
/*
//int32_t Encoder1_Get_Counter(void)
//{
//	return TIM2->CNT + Num_Encoder_1;
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM2)
//	{
//		if(TIM2->CR1 & 0x10)
//		{
//			Num_Encoder_1 -= 0xFFFF;
//		}
//		else
//		{
//			Num_Encoder_1 += 0xFFFF;
//		}
//	}
//}*/
void RunLeft(){
//	uint32_t Num_Encoder_pre = 0, Num_Encoder_next = 0;
//	Num_Encoder_pre = Encoder1_Get_Counter();
////	printf ("pre left  %d\n", Num_Encoder_pre);
//	while (1){
//		Num_Encoder_next = Encoder1_Get_Counter();
//		setAngle(0);
//		RightMotor(1, _speed);
//		LeftMotor(1, _speed);	
//		if((Num_Encoder_next - Num_Encoder_pre) > NumPreRunLeft){
////			printf ("next right wait %d\n", (Num_Encoder_next - Num_Encoder_pre));
//			break;
//		}
//	}
//	
//#if (!tim3_encoder_read_ok)
//	Num_Encoder_pre = Encoder1_Get_Counter();
//	Num_Encoder_next = Encoder1_Get_Counter();
//	while (1){
////		setAngle(55);
//		setAngle(AngleLeft);
//		RightMotor(1, _speed);
//		LeftMotor(1, _speed*0.45);
//		Num_Encoder_next = Encoder1_Get_Counter();
//		if ((Num_Encoder_next - Num_Encoder_pre) > NumEncoderLeft*0.6){
//			setAngle(0);
//			RightMotor(1, 0);
//			LeftMotor(1, 0);
////			printf ("next right  %d\n", Num_Encoder_next);
//			break;
//		}
//	}

//#else

//	Num_Encoder_pre = Encoder2_Get_Counter();
//	Num_Encoder_next = Encoder2_Get_Counter();
//	while (1){
////		setAngle(-33);
//		setAngle(AngleLeft);
//		RightMotor(1, _speed);
//		LeftMotor(1, _speed*0.45);
//		
//		Num_Encoder_next = Encoder2_Get_Counter();
//		
//		if ((Num_Encoder_pre - Num_Encoder_next) > NumEncoderLeft){
//			setAngle(0);
//			RightMotor(1, 0);
//			LeftMotor(1, 0);
////			printf ("next left  %d\n", Num_Encoder_next);
//			break;
//		}
//	}
//	
//	
//#endif
////	while (1){
////		Num_Encoder_next = Encoder2_Get_Counter();
////		setAngle(0);
////		RightMotor(1, _speed);
////		LeftMotor(1, _speed);
//////		printf("wait...");
////		if((Num_Encoder_pre - Num_Encoder_next) > NumPreRunLeft){
////			break;
////		}
////	}
////	Num_Encoder_pre = Encoder2_Get_Counter();
////	Num_Encoder_next = Encoder2_Get_Counter();
////	while (1){
//////		setAngle(-33);
////		setAngle(AngleLeft);
////		RightMotor(1, _speed);
////		LeftMotor(1, _speed*0.45);
////		
////		Num_Encoder_next = Encoder2_Get_Counter();
////		
////		if ((Num_Encoder_pre - Num_Encoder_next) > NumEncoderLeft){
////			setAngle(0);
////			RightMotor(1, 0);
////			LeftMotor(1, 0);
//////			printf ("next left  %d\n", Num_Encoder_next);
////			break;
////		}
////	}
//	State = Straight;
}


void RunRight(){
//	uint32_t Num_Encoder_pre = 0, Num_Encoder_next = 0;
//	Num_Encoder_pre = Encoder1_Get_Counter();
////	printf ("pre right %d\n", Num_Encoder_pre);
//	while (1){
//		Num_Encoder_next = Encoder1_Get_Counter();
//		setAngle(0);
//		RightMotor(1, _speed);
//		LeftMotor(1, _speed);	
//		if((Num_Encoder_next - Num_Encoder_pre) > NumPreRunRight){
////			printf ("next right wait %d\n", (Num_Encoder_next - Num_Encoder_pre));
//			break;
//		}
//	}
//	Num_Encoder_pre = Encoder1_Get_Counter();
//	Num_Encoder_next = Encoder1_Get_Counter();
//	while (1){
////		setAngle(55);
//		setAngle(AngleRight);
//		RightMotor(1, _speed*0.40);
//		LeftMotor(1, _speed);
//		Num_Encoder_next = Encoder1_Get_Counter();
//		if ((Num_Encoder_next - Num_Encoder_pre) > NumEncoderRight){
//			setAngle(0);
//			RightMotor(1, 0);
//			LeftMotor(1, 0);
////			printf ("next right  %d\n", Num_Encoder_next);
//			break;
//		}
//	}
//	State = Straight;
}
double pid_Left(double speed_L, double duty_L, float kp_L, float ki_L, float kd_L)
{
//		speed_L=speed_L/60;
		int count_value = Encoder1_Get_Counter()-32767;
		
		read_encoder1=count_value;
		error = ((speed_L)*500-count_value*10)/100;
//		duty_L=duty_L+error*0.001;
		i_L=i_L+error;
		d_L=(error-preError_L);
		duty_L=duty_L+(kp_L*error)+(ki_L*i_L)+(kd_L*d_L);
		preError_L=error;
		//luu data
//		data_s[ds]=ds;
//		data_duty[ds]=duty;
//		d_motor[ds]=data_motor;
//		d_value[ds]=value;
//		data_count[ds]=count_value;
		precount_L=count_value;
	
	return duty_L;
}
double pid_Right(double speed_R, double duty_R, float kp_R, float ki_R, float kd_R)
{
		int count_value = Encoder2_Get_Counter()-32767;// for some motor
//		int count_value = 32767-Encoder2_Get_Counter();
		read_encoder2=count_value;
//		precount_R=precount_R-(LPF_Beta*(precount_R-count_value));
//	  count_value=precount_R;
		double error_R = ((speed_R)*50-count_value)/10;
		i_R=i_R+error_R;
		d_R=(error_R-preError_R);
		duty_R=duty_R+(kp_R*error_R)+(ki_R*i_R)+(kd_R*d_R);
		preError_R=error_R;
		//luu data
//		data_s[ds]=ds;
//		data_duty[ds]=duty;
//		d_motor[ds]=data_motor;
//		d_value[ds]=value;
//		data_count[ds]=count_value;
		precount_R=count_value;
	
	return duty_R;
}
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 10);
    return ch;
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
	printf("\n\rError!\n\r");
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
