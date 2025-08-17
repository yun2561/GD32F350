/*!
    \file    Uart.h
    \brief   header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#include "gd32f3x0_libopt.h"

#ifndef	__SYS_UART_H__
#define __SYS_UART_H__

#include "gd32f3x0_libopt.h"
typedef enum {
    UART_DMA_STATUS_IDLE = 0,
    UART_DMA_STATUS_BUSY,
    UART_DMA_STATUS_COMPLETE,
    UART_DMA_STATUS_ERROR,
    UART_DMA_STATUS_ABORTED
} uart_dma_status_t;

// UART DMA 错误码定义
typedef enum {
    UART_DMA_ERROR_NONE = 0,
    UART_DMA_ERROR_INVALID_PARAM,
    UART_DMA_ERROR_NOT_INITIALIZED,
    UART_DMA_ERROR_BUSY,
    UART_DMA_ERROR_QUEUE_FULL,
    UART_DMA_ERROR_QUEUE_EMPTY,
    UART_DMA_ERROR_DMA_ERROR,
    UART_DMA_ERROR_UART_ERROR,
    UART_DMA_ERROR_TIMEOUT
} uart_dma_error_t;

typedef struct {
    uint32_t uart_periph;
    uint32_t uart_baudrate;
    uint32_t word_lenght;
    uint32_t stop_bit;
    uint32_t parity;
    uint32_t hardware_flow;       
} uart_hw_config_t;

typedef struct{
    uint32_t dma_periph;
    dma_channel_enum tx_channel;
    dma_channel_enum rx_channel;
    uint32_t tx_prority;
    uint32_t rx_perority; 
} uart_dma_hw_config_t;

// 错误恢复配置结构
typedef struct {
    bool auto_recovery_enabled;
    uint8_t max_retry_count;
    uint32_t recovery_timeout_ms;
    void (*error_callback)(uart_dma_error_t error);
} uart_dma_error_config_t;

typedef struct 
{
    uart_hw_config_t uart_config;
    uart_dma_hw_config_t dma_config;
    uart_dma_error_config_t error_config;
    uint16_t tx_fifo_size;
    uint16_t rx_fifo_size;
    uint16_t ping_pong_size;   
    bool enable_circular_mode;
    bool enable_error_recovery; 
} uart_dma_config_t;

typedef struct 
{
    uint8_t * buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
    uint16_t count;
    bool is_full;
} fifo_queue_t;
typedef struct
{
    uint8_t* buffer;
    uint16_t size;
    uint16_t write_index;
    uint16_t read_index;
    bool is_full;
} ring_buffer_t;

// 回调函数类型定义
typedef void (*uart_dma_callback_t)(uart_dma_status_t status, uint16_t length);

/*Uart init*/
void UART_DMA_Init(void);
/*Uart init*/
void UART_Init(void);

#endif