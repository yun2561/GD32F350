/*!
    \file    LED.c
    \brief   led file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "Key.h"

#define 	K0 gpio_input_bit_get(KEY_0_GPIO,KEY_0_PIN)
#define 	K1 gpio_input_bit_get(KEY_1_GPIO,KEY_1_PIN)

int32_t 	K0Cnt, K1Cnt;



/*!
    \brief      led gpio rcu clock enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void KEY_GPIO_RCU_Clock_Enable(void)
{
    /*led_1 rcu clock enable*/
    rcu_periph_clock_enable(KEY_0_RCU);
    /*led_2 rcu clock enable*/
    rcu_periph_clock_enable(KEY_1_RCU);


}
/*!
    \brief      led gpio configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void KEY_GPIO_Configure(void)
{

    /*led rcu clock enable*/
    KEY_GPIO_RCU_Clock_Enable();

    /*KEY0 port mode configure*/
    gpio_mode_set(
        KEY_0_GPIO,
        GPIO_MODE_INPUT,
        GPIO_PUPD_PULLUP,
        KEY_0_PIN);
    /*led1 off*/
    //gpio_bit_set(LED_1_GPIO,LED_1_PIN);


    /*KEY1 port mode configure*/
    gpio_mode_set(KEY_1_GPIO,
                  GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLUP,
                  KEY_1_PIN);
    /*led0 off*/
    //gpio_bit_set(LED_1_GPIO,LED_1_PIN);
    //gpio_bit_set(LED_0_GPIO,LED_0_PIN);


}

int32_t KEY_Scan(int32_t PIN, int32_t *kCnt)
{
    /*get key status*/
    int32_t k = PIN;
    /*judge key whether pressed case*/
    //if((0 == k) && (999999 != *kCnt) && (++*kCnt > 10))
    if((0 == k) && (999999 != *kCnt))
    {
        /*judge key whether pressed case*/
        if(0 == k)
        {
            /*set key pressed flag*/
            *kCnt = 999999;
            /*return key pressed status*/
            return 0;
        }
    }
    /*judge key whether relesaed*/
    else if(0 != k)
    {
        /*judge previous key flag*/
        if(999999 == *kCnt)
        {
            /*clear key count*/
            *kCnt = 0;
            /*return released status*/
            return 1;
        }
    }
    /*return invalid status*/
    return -1;

}

void KEY_Process(void)
{
    /*Identify KI*/
    if(0 == KEY_Scan(K0, &K0Cnt))
    {
        /*led1 on*/
        gpio_bit_reset(LED_0_GPIO, LED_0_PIN);
        /*led2 off*/
        gpio_bit_set(LED_1_GPIO, LED_1_PIN);
}
    /*Identify K2*/
    if(0 == KEY_Scan(K1, &K1Cnt))
    {
        /*led2 on*/
        gpio_bit_reset(LED_1_GPIO, LED_1_PIN);
        /*led1 off*/
        gpio_bit_set(LED_0_GPIO, LED_0_PIN);
    }
}
static uint32_t key_timer_cnt=key_scan_time;

void SysTick_KEY_Scan(void)
{
    
    //uint16_t curKeySta;
    //uint16_t temp;
    //static uint16_t   preKeySta = 0;
    //temp=(KEY_0_PIN|KEY_1_PIN);
    if(SysTickTimer(&key_timer_cnt,SYS_KEY_TIMER_BIT))
    {
        key_timer_cnt=key_scan_time;
        if(0 == KEY_Scan(K0, &K0Cnt))
        {
            gpio_bit_write(LED_0_GPIO,LED_0_PIN,(bit_status)!gpio_input_bit_get(LED_0_GPIO,LED_0_PIN)); 
        }
        if(0 == KEY_Scan(K1, &K1Cnt))
        {
            gpio_bit_write(LED_1_GPIO,LED_1_PIN,(bit_status)!gpio_input_bit_get(LED_1_GPIO,LED_1_PIN)); 
        }
        /*curKeySta =gpio_input_port_get(KEY_0_GPIO)& (KEY_0_PIN|KEY_1_PIN);
        if(preKeySta!=(curKeySta))
        {
            
            //temp=preKeySta&KEY_0_PIN;
            if((RESET==K0)&&(KEY_0_PIN==(preKeySta&KEY_0_PIN)))
            {
                gpio_bit_write(LED_0_GPIO,LED_0_PIN,(bit_status)!gpio_input_bit_get(LED_0_GPIO,LED_0_PIN)); 
            }
            if((RESET==K1)&(KEY_1_PIN==(preKeySta&KEY_1_PIN)))
            {
                gpio_bit_write(LED_1_GPIO,LED_1_PIN,(bit_status)!gpio_input_bit_get(LED_1_GPIO,LED_1_PIN)); 
            }
            preKeySta=curKeySta ;

        }*/
       
    }
}



