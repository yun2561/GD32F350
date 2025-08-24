/*!
    \file    Uart.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/


#ifndef	__SYS_FIFO_H__
#define __SYS_FIFO_H__

#include "gd32f3x0_libopt.h"
// FIFO 错误码定义
typedef enum {
    FIFO_ERROR_NONE = 0,
    FIFO_ERROR_INVALID_PARAM,
    FIFO_ERROR_NOT_INITIALIZED,
    FIFO_ERROR_BUSY,
    FIFO_ERROR_QUEUE_FULL,
    FIFO_ERROR_QUEUE_EMPTY,
    FIFO_ERROR_DMA_ERROR,
    FIFO_ERROR_UART_ERROR,
    FIFO_ERROR_TIMEOUT
} fifo_error_t;

#endif
