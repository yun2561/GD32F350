/*!
    \file    main.c
    \brief   Key-Ex-3

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "main.h"


int main(void)
{
    
    Gpio_Init();
    systick_config();
    gSysVarInit();
    Timer0_PWM_Init();
    uint32_t Duty_Cycle=0;
    FlagStatus breathe_flag=SET;;
    
    
    while(1)
    {   //delay_1ms(40);
        //KEY_Process();
        if(SET==(gSysVar.tick_flag1ms))
        {
            delay_1ms(50);
            gSysVar.tick_flag1ms=0;
            //SysTick_KEY_Scan();
            if(breathe_flag==SET)
            {
                Duty_Cycle=Duty_Cycle+10;
            }
            else
            {
                Duty_Cycle=Duty_Cycle-10;
            }
            if(Duty_Cycle>500)
            {
                breathe_flag=RESET;
            }
            if(Duty_Cycle<=0)
            {
                breathe_flag=SET;
            }
            timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,Duty_Cycle)  ;
        }
        
    }
   
}

