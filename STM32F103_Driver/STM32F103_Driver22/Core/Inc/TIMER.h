/*
 * TIMER.h
 *
 *  Created on: Nov 13, 2020
 *      Author: quanp
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private defines -----------------------------------------------------------*/
#define Timer_Servo_Period			17999	//Exact Period = Period + 1 = 18000
#define Timer_Servo_Prescaler 	79		//Exact Prescaler = Prescaler + 1 = 80
#define Timer_Servo_Base				450
#define Timer_Servo_Center			1350
#define Timer_Servo_Top					2250
#define Timer_Servo_Step_Angle	10
#define Neg_Servo_Angle					-55
#define Pos_Servo_Angle					55
/* Exported functions prototypes ---------------------------------------------*/
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);

void setPWM(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t period, uint32_t pulse);
#endif /* INC_TIMER_H_ */
