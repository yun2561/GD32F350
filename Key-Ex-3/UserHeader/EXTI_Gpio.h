/*!
    \file    EXTI_Gpio.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#ifndef	__EXTI_GPIO_H__
#define __EXTI_GPIO_H__

#include "gd32f3x0_libopt.h"

#include "LED.h"

#define		EXTI_1_RCU			RCU_GPIOC
#define 	EXTI_1_GPIO			GPIOC
#define		EXTI_1_PIN			GPIO_PIN_1
#define		EXTI_SOURCE_1_PIN	        EXTI_SOURCE_PIN1

#define		EXTI_13_RCU			RCU_GPIOC
#define 	EXTI_13_GPIO		GPIOC
#define		EXTI_13_PIN			GPIO_PIN_13
#define		EXTI_SOURCE_13_PIN	        EXTI_SOURCE_PIN13

/*EXTI0 gpio rcu clock enable*/
void EXTI1_GPIO_RCU_Clock_Enable(void);

/*EXTI0 gpio init*/
void EXTI1_GPIO_Init(void);
	
/*EXT0 Init*/
void EXTI1_Init(void);

/*EXTI13 gpio rcu clock enable*/
void EXTI13_GPIO_RCU_Clock_Enable(void);

/*EXTI13 gpio init*/
void EXTI13_GPIO_Init(void);
	
/*EXT13 Init*/
void EXTI13_Init(void);

/*EXT13 Init*/
void EXTI_Configure(void);
	
#endif


