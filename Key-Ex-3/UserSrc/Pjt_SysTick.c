/*!
    \file    EXTI_Gpio.c
    \brief   EXTI_Gpio file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "Pjt_SysTick.h"

uint32_t flag1ms=0;

/*!
    \brief      SysTick_Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/

static uint32_t flag1ms;
void SysTick_Interrupt(void)
{
    flag1ms=0xffffffff;
    gSysVar.tick_flag1ms=1;
}

/*!
    \brief      SysTick_Handler
    \param[in]  none
    \param[out] none
    \retval     none
*/

uint32_t SysTickTimer(uint32_t *Cnt,uint32_t bit)
{
    if(*Cnt!=0)
    {
        if(flag1ms&bit)
        {
            flag1ms&=~bit;
            (*Cnt)--;
        }
        if(*Cnt==0)
        {
            return TRUE;
        }
        
    }
    return FALSE;
}



