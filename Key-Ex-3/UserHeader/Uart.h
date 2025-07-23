/*!
    \file    Uart.h
    \brief   UART DMA communication system header file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#ifndef __UART_H__
#define __UART_H__

#include "gd32f3x0_libopt.h"
#include <stdbool.h>
#include <stdint.h>

// 缓冲区大小定义
#define RX_FIFO_SIZE    512
#define TX_FIFO_SIZE    256
#define PING_PONG_SIZE  128

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

// UART DMA 状态定义
typedef enum {
    UART_DMA_STATUS_IDLE = 0,
    UART_DMA_STATUS_BUSY,
    UART_DMA_STATUS_COMPLETE,
    UART_DMA_STATUS_ERROR,
    UART_DMA_STATUS_ABORTED
} uart_dma_status_t;

// FIFO队列结构
typedef struct {
    uint8_t* buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
    uint16_t count;
    bool is_full;
} fifo_queue_t;

// 环形缓冲区结构
typedef struct {
    uint8_t* buffer;
    uint16_t size;
    uint16_t write_index;
    uint16_t read_index;
    bool is_full;
} ring_buffer_t;

// UART硬件配置结构
typedef struct {
    uint32_t uart_periph;       // UART外设 (USART0/USART1)
    uint32_t baudrate;          // 波特率
    uint32_t word_length;       // 数据位
    uint32_t stop_bit;          // 停止位
    uint32_t parity;            // 校验位
    uint32_t hardware_flow;     // 硬件流控
} uart_hw_config_t;

// DMA硬件配置结构
typedef struct {
    uint32_t dma_periph;        // DMA外设 (DMA0)
    dma_channel_enum tx_channel; // 发送通道 (DMA_CH1)
    dma_channel_enum rx_channel; // 接收通道 (DMA_CH2)
    uint32_t tx_priority;       // 发送优先级
    uint32_t rx_priority;       // 接收优先级
    bool circular_mode;         // 循环模式使能
} uart_dma_hw_config_t;

// 错误恢复配置结构
typedef struct {
    bool auto_recovery_enabled;
    uint8_t max_retry_count;
    uint32_t recovery_timeout_ms;
    void (*error_callback)(uart_dma_error_t error);
} uart_dma_error_config_t;

// UART DMA完整配置结构
typedef struct {
    uart_hw_config_t uart_config;
    uart_dma_hw_config_t dma_config;
    uart_dma_error_config_t error_config;
    uint16_t tx_fifo_size;
    uint16_t rx_fifo_size;
    uint16_t ping_pong_size;
    bool enable_circular_mode;
    bool enable_error_recovery;
} uart_dma_config_t;

// 统计信息结构
typedef struct {
    uint32_t tx_bytes_total;
    uint32_t rx_bytes_total;
    uint32_t tx_packets_total;
    uint32_t rx_packets_total;
    uint32_t tx_errors;
    uint32_t rx_errors;
    uint32_t queue_overflows;
    uint32_t dma_errors;
} uart_dma_statistics_t;

// 回调函数类型定义
typedef void (*uart_dma_callback_t)(uart_dma_status_t status, uint16_t length);

// ============================================================================
// API 接口声明
// ============================================================================

// 初始化和配置接口
uart_dma_error_t uart_dma_init(uart_dma_config_t* config);
uart_dma_error_t uart_dma_deinit(void);

// 发送接口
uart_dma_error_t uart_dma_send(const uint8_t* data, uint16_t length);
uart_dma_error_t uart_dma_send_async(const uint8_t* data, uint16_t length, uart_dma_callback_t callback);

// 接收接口
uart_dma_error_t uart_dma_receive(uint8_t* buffer, uint16_t length);
uart_dma_error_t uart_dma_receive_async(uint8_t* buffer, uint16_t length, uart_dma_callback_t callback);

// 状态查询接口
uart_dma_status_t uart_dma_get_tx_status(void);
uart_dma_status_t uart_dma_get_rx_status(void);
uint16_t uart_dma_get_tx_queue_count(void);
uint16_t uart_dma_get_rx_queue_count(void);

// 控制接口
uart_dma_error_t uart_dma_abort_tx(void);
uart_dma_error_t uart_dma_abort_rx(void);
uart_dma_error_t uart_dma_flush_tx_queue(void);
uart_dma_error_t uart_dma_flush_rx_queue(void);

// 统计和调试接口
uart_dma_statistics_t* uart_dma_get_statistics(void);
void uart_dma_reset_statistics(void);

// 传统接口保持兼容
void Uart_Init(void);

#endif