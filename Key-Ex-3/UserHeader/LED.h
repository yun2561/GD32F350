/*!
    \file    LED.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#ifndef	__LED_H__
#define __LED_H__

#include "gd32f3x0_libopt.h"

#define	LED_0_RCU	RCU_GPIOA
#define LED_0_GPIO  GPIOA
#define	LED_0_PIN	GPIO_PIN_8

#define		LED_1_RCU			RCU_GPIOD
#define 	LED_1_GPIO		    GPIOD
#define		LED_1_PIN			GPIO_PIN_2


/*led gpio rcu clock enable*/
void LED_GPIO_RCU_Clock_Enable(void);


/*configure led gpio*/
void LED_GPIO_Configure(void);


#endif

