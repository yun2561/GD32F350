/*!
    \file    Gpio.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#ifndef	__KEY_H__
#define __KEY_H__

#include "gd32f3x0_libopt.h"

#include "LED.h"

#include "systick.h"
#include "Pjt_SysTick.h"


#define		KEY_0_RCU			RCU_GPIOC
#define 	KEY_0_GPIO		GPIOC
#define		KEY_0_PIN			GPIO_PIN_8

#define		KEY_1_RCU			RCU_GPIOC
#define 	KEY_1_GPIO		GPIOC
#define		KEY_1_PIN			GPIO_PIN_6

#define key_scan_time   ((uint32_t)50)


/*key gpio rcu clock enable*/
void KEY_GPIO_RCU_Clock_Enable(void);


/*configure key gpio*/
void KEY_GPIO_Configure(void);

/*key scan*/
int32_t KEY_Scan(int32_t PIN,int32_t *kCnt);

void KEY_Process(void);

void SysTick_KEY_Scan(void);

#endif

