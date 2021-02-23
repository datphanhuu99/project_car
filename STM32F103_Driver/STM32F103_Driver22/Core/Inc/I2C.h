/*
 * I2C.h
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_8
#define I2C1_SCL_GPIO_Port GPIOB

#define I2C2_SDA_Pin GPIO_PIN_11
#define I2C2_SDA_GPIO_Port GPIOB
#define I2C2_SCL_Pin GPIO_PIN_10
#define I2C2_SCL_GPIO_Port GPIOB

/* Exported functions prototypes ---------------------------------------------*/
void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

#endif /* INC_I2C_H_ */
