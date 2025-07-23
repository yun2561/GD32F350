/*!
    \file    Gpio.c
    \brief   Gpio file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#include "Gpio.h"

/*!
    \brief      gpio init
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Gpio_Init(void)
{
    /*led gpio config*/
    LED_GPIO_Configure();

    /*key gpio config*/
    //KEY_GPIO_Configure();

    /*EXTI gpio config*/
    //EXTI_Configure();

}


