/*!
    \file    gSysVar.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/


#ifndef	__GSYSVAR_H__
#define __GSYSVAR_H__

#include "gd32f3x0_libopt.h"


typedef struct
{
    uint32_t tick_flag1ms;
    
}gVarClass;

extern gVarClass     gSysVar;

void gSysVarInit(void);


#endif


