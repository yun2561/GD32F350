/*!
    \file    LED.c
    \brief   led file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "LED.h"

/*!
    \brief      led gpio rcu clock enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void LED_GPIO_RCU_Clock_Enable(void)
{
    /*led_1 rcu clock enable*/
    rcu_periph_clock_enable(LED_0_RCU);
    /*led_2 rcu clock enable*/
    rcu_periph_clock_enable(LED_1_RCU);


}
/*!
    \brief      led gpio configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void LED_GPIO_Configure(void)
{

    /*led rcu clock enable*/
    LED_GPIO_RCU_Clock_Enable();

    /*led0 port mode configure*/
    //gpio_mode_set(LED_0_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED_0_PIN);
    
    gpio_mode_set(LED_0_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_0_PIN);
    
    
    
    
    
    /*led0 port output configure*/
    gpio_output_options_set(LED_0_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_0_PIN);
    /*led0 off*/
    //gpio_bit_set(LED_0_GPIO, LED_0_PIN);

    gpio_af_set(LED_0_GPIO,GPIO_AF_2,LED_0_PIN);




    /*led1 port mode configure*/
    gpio_mode_set(LED_1_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, LED_1_PIN);

    /*led1 port output configure*/
    gpio_output_options_set(LED_1_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_1_PIN);

    /*led1 off*/
    gpio_bit_set(LED_1_GPIO, LED_1_PIN);

}

