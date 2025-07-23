/*!
    \file    Pjt_SysTick.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#ifndef	__PRT_SYSTICK_H__
#define __PRT_SYSTICK_H__

#include "gd32f3x0_libopt.h"

#include "gSysVar.h"

#define SYS_TICK_TIMER_BIT			((uint32_t)(1u<<0))
#define SYS_KEY_TIMER_BIT			((uint32_t)(1u<<1))


#define SYS_KEY_SCAN_Timer          20

void SysTick_Interrupt(void);
uint32_t SysTickTimer(uint32_t *Cnt,uint32_t bit);
#endif

