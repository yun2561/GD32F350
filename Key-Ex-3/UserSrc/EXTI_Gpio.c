/*!
    \file    EXTI_Gpio.c
    \brief   EXTI_Gpio file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "EXTI_Gpio.h"


/*!
    \brief      EXTI0 gpio rcu clock enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI1_GPIO_RCU_Clock_Enable(void)
{
    /*EXTI_0 rcu CFGCMP clock enable*/
    rcu_periph_clock_enable(RCU_CFGCMP);
    /*EXTI_0 rcu GPIO clock enable*/
    rcu_periph_clock_enable(EXTI_1_RCU);



}


/*!
    \brief      EXTI0 gpio init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI1_GPIO_Init(void)
{
    /*EXTI0 rcu clock enable*/
    EXTI1_GPIO_RCU_Clock_Enable();

    /*EXTI0 port mode configure*/
    /*gpio_mode_set(EXTI_0_GPIO,
    							GPIO_MODE_INPUT,
    							GPIO_PUPD_PULLUP,
    							EXTI_0_PIN);
    */
    gpio_mode_set(EXTI_1_GPIO,
                  GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLUP,
                  EXTI_1_PIN);

}
/*!
    \brief      EXTI0 Init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI1_Init(void)
{
    /*EXTI0 port init*/
    EXTI1_GPIO_Init();
    /*config exti_1 source*/
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC,EXTI_SOURCE_1_PIN);
    /*EXTI0 configure*/
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    /*clear EXTI0 flag*/
    exti_interrupt_flag_clear(EXTI_1);
    /*enable EXTI0*/
    exti_interrupt_enable(EXTI_1);
    /*EXT0 interrupt enable*/
    nvic_irq_enable(EXTI0_1_IRQn, 0U, 0U);




}

/*!
    \brief      EXTI13 gpio rcu clock enable
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI13_GPIO_RCU_Clock_Enable(void)
{
    /*EXTI_0 rcu CFGCMP clock enable*/
    rcu_periph_clock_enable(RCU_CFGCMP);
    /*EXTI_1 rcu clock enable*/
    rcu_periph_clock_enable(EXTI_13_RCU);



}
/*!
    \brief      EXTI13 gpio init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI13_GPIO_Init(void)
{
    /*EXTI rcu clock enable*/
    EXTI13_GPIO_RCU_Clock_Enable();

    /*EXTI1 port mode configure*/
    gpio_mode_set(EXTI_13_GPIO,
                  GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLUP,
                  EXTI_13_PIN);
}
/*!
    \brief      EXTI13 init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI13_Init(void)
{
    /*EXTI1 port init*/
    EXTI13_GPIO_Init();
    /*config exti_13 source*/
    syscfg_exti_line_config(EXTI_SOURCE_GPIOC,EXTI_SOURCE_13_PIN);
    /*EXTI1 configure*/
    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    /*clear EXTI13 flag*/
    exti_interrupt_flag_clear(EXTI_13);
    /*enable EXTI13*/
    exti_interrupt_enable(EXTI_13);
    /*EXT1 interrupt enable*/
    nvic_irq_enable(EXTI4_15_IRQn, 0U, 1U);




}
void EXTI_Configure(void)
{
    /*EXTI0_Init*/
    EXTI1_Init();
    /*EXTI13_Init*/
    EXTI13_Init();
}
void EXTI0_1_IRQHandler(void)
{
    exti_interrupt_flag_clear(EXTI_1);
    /*led1 on*/
    //gpio_bit_reset(LED_0_GPIO, LED_0_PIN);
    gpio_bit_write(LED_0_GPIO,LED_0_PIN,(bit_status)!gpio_input_bit_get(LED_0_GPIO,LED_0_PIN));
    
    /*led2 off*/
    //gpio_bit_set(LED_1_GPIO, LED_1_PIN);
}

void EXTI4_15_IRQHandler(void)
{		
	exti_interrupt_flag_clear(EXTI_13);
    /*led2 on*/
    //gpio_bit_reset(LED_1_GPIO, LED_1_PIN);
	gpio_bit_write(LED_1_GPIO,LED_1_PIN,(bit_status)!gpio_input_bit_get(LED_1_GPIO,LED_1_PIN));
    /*led1 off*/
    //gpio_bit_set(LED_0_GPIO, LED_0_PIN);
}

