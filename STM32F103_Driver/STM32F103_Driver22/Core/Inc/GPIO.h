/*
 * GPIO.h
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define Button1_Pin						GPIO_PIN_12
#define Button1_GPIO_Port 		GPIOB
#define Button2_Pin						GPIO_PIN_13
#define Button2_GPIO_Port 		GPIOB
#define Button3_Pin						GPIO_PIN_14
#define Button3_GPIO_Port 		GPIOB
#define Button4_Pin						GPIO_PIN_15
#define Button4_GPIO_Port 		GPIOB
#define Servo_Pin 						GPIO_PIN_1
#define Servo_GPIO_Port 			GPIOA

#define MotorLeft_Dir_pin 				GPIO_PIN_1
#define MotorLeft_Dir_GPIO_Port 	GPIOB
#define MotorRight_Dir_pin 				GPIO_PIN_0
#define MotorRight_Dir_GPIO_Port 	GPIOB
/* Exported functions prototypes ---------------------------------------------*/
void MX_GPIO_Init(void);

#endif /* INC_GPIO_H_ */
