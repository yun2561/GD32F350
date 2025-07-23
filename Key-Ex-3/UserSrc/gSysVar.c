/*!
    \file    gSysVar.c
    \brief   c file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#include "gSysVar.h"

gVarClass     gSysVar  = {0};

void gSysVarInit(void)
{
    gSysVar.tick_flag1ms=0;

}

