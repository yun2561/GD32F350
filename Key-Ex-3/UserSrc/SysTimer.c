/*!
    \file    SysTimer.c
    \brief   c file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#include "SysTimer.h"

void Timer0_PWM_Init(void)
{
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocintpara;
    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);
    timer_initpara.prescaler=119;
    timer_initpara.alignedmode=TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection=TIMER_COUNTER_UP;
    timer_initpara.period            = 500;
    timer_initpara.clockdivision=TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter=0;
    
    timer_init(TIMER0,&timer_initpara);
    
    
    timer_ocintpara.outputstate=TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate=TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity=TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity=TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate=TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate=TIMER_OCN_IDLE_STATE_LOW;
    
    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
    
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,250);
    
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);
    
    timer_primary_output_config(TIMER0,ENABLE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
    
    
}



