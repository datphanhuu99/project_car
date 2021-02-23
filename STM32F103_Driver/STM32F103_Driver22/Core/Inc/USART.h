/*
 * USART.h
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

#ifndef INC_USART_H_
#define INC_USART_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define UART2_RX_Pin 				GPIO_PIN_3
#define UART2_RX_GPIO_Port 	GPIOA
#define UART2_TX_Pin 				GPIO_PIN_2
#define UART2_TX_GPIO_Port 	GPIOA

/* Exported functions prototypes ---------------------------------------------*/
void MX_USART2_Init(void);

#endif /* INC_USART_H_ */
