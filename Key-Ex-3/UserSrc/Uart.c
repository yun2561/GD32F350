/*!
    \file    Uart.c
    \brief   UART DMA communication system implementation

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/

#include "Uart.h"
#include <string.h>

// ============================================================================
// 内部数据结构和变量
// ============================================================================

// FIFO队列管理器
typedef struct {
    fifo_queue_t tx_queue;
    fifo_queue_t rx_queue;
    uint8_t tx_buffer[TX_FIFO_SIZE];
    uint8_t rx_buffer[RX_FIFO_SIZE];
} uart_fifo_manager_t;

// 环形缓冲区管理器
typedef struct {
    ring_buffer_t tx_ring;
    ring_buffer_t rx_ring;
    uint8_t tx_ping_buffer[PING_PONG_SIZE];
    uint8_t tx_pong_buffer[PING_PONG_SIZE];
    uint8_t rx_ping_buffer[PING_PONG_SIZE];
    uint8_t rx_pong_buffer[PING_PONG_SIZE];
    bool tx_using_ping;
    bool rx_using_ping;
} uart_ring_manager_t;

// 系统状态管理器
typedef struct {
    uart_dma_status_t tx_status;
    uart_dma_status_t rx_status;
    bool initialized;
    uart_dma_config_t config;
    uart_dma_statistics_t statistics;
    uart_dma_callback_t tx_callback;
    uart_dma_callback_t rx_callback;
} uart_system_state_t;

// 全局变量
static uart_fifo_manager_t g_fifo_manager;
static uart_ring_manager_t g_ring_manager;
static uart_system_state_t g_system_state = {0};

// ============================================================================
// 内部函数声明
// ============================================================================

// FIFO队列操作函数
static uart_dma_error_t fifo_init(fifo_queue_t* fifo, uint8_t* buffer, uint16_t size);
static uart_dma_error_t fifo_enqueue(fifo_queue_t* fifo, const uint8_t* data, uint16_t length);
static uart_dma_error_t fifo_dequeue(fifo_queue_t* fifo, uint8_t* data, uint16_t* length);
static uint16_t fifo_get_count(fifo_queue_t* fifo);
static uart_dma_error_t fifo_flush(fifo_queue_t* fifo);

// 环形缓冲区操作函数
static uart_dma_error_t ring_buffer_init(ring_buffer_t* ring, uint8_t* buffer, uint16_t size);
static uart_dma_error_t ring_buffer_write(ring_buffer_t* ring, const uint8_t* data, uint16_t length);
static uart_dma_error_t ring_buffer_read(ring_buffer_t* ring, uint8_t* data, uint16_t* length);

// 硬件抽象层函数
static uart_dma_error_t uart_hw_init(uart_hw_config_t* config);
static uart_dma_error_t uart_hw_deinit(void);
static uart_dma_error_t dma_hw_init(uart_dma_hw_config_t* config);
static uart_dma_error_t dma_hw_deinit(void);

// DMA控制函数
static uart_dma_error_t dma_start_tx(const uint8_t* data, uint16_t length);
static uart_dma_error_t dma_start_rx(uint8_t* buffer, uint16_t length);
static uart_dma_error_t dma_stop_tx(void);
static uart_dma_error_t dma_stop_rx(void);

// 中断处理函数
void DMA0_Channel1_IRQHandler(void);  // TX DMA中断
void DMA0_Channel2_IRQHandler(void);  // RX DMA中断
void USART0_IRQHandler(void);         // UART中断

// ============================================================================
// API接口实现
// ============================================================================

/*!
    \brief      初始化UART DMA系统
    \param[in]  config: 配置参数指针
    \retval     uart_dma_error_t: 错误码
*/
uart_dma_error_t uart_dma_init(uart_dma_config_t* config)
{
    if (config == NULL) {
        return UART_DMA_ERROR_INVALID_PARAM;
    }
    
    if (g_system_state.initialized) {
        return UART_DMA_ERROR_BUSY;
    }
    
    // 保存配置
    memcpy(&g_system_state.config, config, sizeof(uart_dma_config_t));
    
    // 初始化FIFO队列
    fifo_init(&g_fifo_manager.tx_queue, g_fifo_manager.tx_buffer, config->tx_fifo_size);
    fifo_init(&g_fifo_manager.rx_queue, g_fifo_manager.rx_buffer, config->rx_fifo_size);
    
    // 初始化环形缓冲区
    ring_buffer_init(&g_ring_manager.tx_ring, g_ring_manager.tx_ping_buffer, config->ping_pong_size);
    ring_buffer_init(&g_ring_manager.rx_ring, g_ring_manager.rx_ping_buffer, config->ping_pong_size);
    
    // 初始化硬件
    uart_dma_error_t result = uart_hw_init(&config->uart_config);
    if (result != UART_DMA_ERROR_NONE) {
        return result;
    }
    
    result = dma_hw_init(&config->dma_config);
    if (result != UART_DMA_ERROR_NONE) {
        uart_hw_deinit();
        return result;
    }
    
    // 初始化状态
    g_system_state.tx_status = UART_DMA_STATUS_IDLE;
    g_system_state.rx_status = UART_DMA_STATUS_IDLE;
    g_system_state.initialized = true;
    
    // 清零统计信息
    memset(&g_system_state.statistics, 0, sizeof(uart_dma_statistics_t));
    
    return UART_DMA_ERROR_NONE;
}

/*!
    \brief      去初始化UART DMA系统
    \retval     uart_dma_error_t: 错误码
*/
uart_dma_error_t uart_dma_deinit(void)
{
    if (!g_system_state.initialized) {
        return UART_DMA_ERROR_NOT_INITIALIZED;
    }
    
    // 停止所有传输
    dma_stop_tx();
    dma_stop_rx();
    
    // 去初始化硬件
    dma_hw_deinit();
    uart_hw_deinit();
    
    // 清空队列
    fifo_flush(&g_fifo_manager.tx_queue);
    fifo_flush(&g_fifo_manager.rx_queue);
    
    // 重置状态
    memset(&g_system_state, 0, sizeof(uart_system_state_t));
    
    return UART_DMA_ERROR_NONE;
}

/*!
    \brief      传统UART初始化接口（保持兼容性）
*/
void Uart_Init(void)
{
    // 使用默认配置初始化UART DMA系统
    uart_dma_config_t default_config = {
        .uart_config = {
            .uart_periph = USART0,
            .baudrate = 115200,
            .word_length = USART_WL_8BIT,
            .stop_bit = USART_STB_1BIT,
            .parity = USART_PM_NONE,
            .hardware_flow = USART_HWFLOW_NONE
        },
        .dma_config = {
            .dma_periph = DMA0,
            .tx_channel = DMA_CH1,
            .rx_channel = DMA_CH2,
            .tx_priority = DMA_PRIORITY_HIGH,
            .rx_priority = DMA_PRIORITY_HIGH,
            .circular_mode = false
        },
        .error_config = {
            .auto_recovery_enabled = true,
            .max_retry_count = 3,
            .recovery_timeout_ms = 1000,
            .error_callback = NULL
        },
        .tx_fifo_size = TX_FIFO_SIZE,
        .rx_fifo_size = RX_FIFO_SIZE,
        .ping_pong_size = PING_PONG_SIZE,
        .enable_circular_mode = false,
        .enable_error_recovery = true
    };
    
    uart_dma_init(&default_config);
}

// ============================================================================
// 占位符实现（后续任务中完善）
// ============================================================================

// 发送接口占位符
uart_dma_error_t uart_dma_send(const uint8_t* data, uint16_t length)
{
    // TODO: 在任务8.2中实现
    return UART_DMA_ERROR_NOT_INITIALIZED;
}

uart_dma_error_t uart_dma_send_async(const uint8_t* data, uint16_t length, uart_dma_callback_t callback)
{
    // TODO: 在任务8.2中实现
    return UART_DMA_ERROR_NOT_INITIALIZED;
}

// 接收接口占位符
uart_dma_error_t uart_dma_receive(uint8_t* buffer, uint16_t length)
{
    // TODO: 在任务8.3中实现
    return UART_DMA_ERROR_NOT_INITIALIZED;
}

uart_dma_error_t uart_dma_receive_async(uint8_t* buffer, uint16_t length, uart_dma_callback_t callback)
{
    // TODO: 在任务8.3中实现
    return UART_DMA_ERROR_NOT_INITIALIZED;
}

// 状态查询接口占位符
uart_dma_status_t uart_dma_get_tx_status(void)
{
    return g_system_state.tx_status;
}

uart_dma_status_t uart_dma_get_rx_status(void)
{
    return g_system_state.rx_status;
}

uint16_t uart_dma_get_tx_queue_count(void)
{
    return fifo_get_count(&g_fifo_manager.tx_queue);
}

uint16_t uart_dma_get_rx_queue_count(void)
{
    return fifo_get_count(&g_fifo_manager.rx_queue);
}

// 控制接口占位符
uart_dma_error_t uart_dma_abort_tx(void)
{
    return dma_stop_tx();
}

uart_dma_error_t uart_dma_abort_rx(void)
{
    return dma_stop_rx();
}

uart_dma_error_t uart_dma_flush_tx_queue(void)
{
    return fifo_flush(&g_fifo_manager.tx_queue);
}

uart_dma_error_t uart_dma_flush_rx_queue(void)
{
    return fifo_flush(&g_fifo_manager.rx_queue);
}

// 统计接口占位符
uart_dma_statistics_t* uart_dma_get_statistics(void)
{
    return &g_system_state.statistics;
}

void uart_dma_reset_statistics(void)
{
    memset(&g_system_state.statistics, 0, sizeof(uart_dma_statistics_t));
}

// ============================================================================
// 内部函数占位符实现（后续任务中完善）
// ============================================================================

static uart_dma_error_t fifo_init(fifo_queue_t* fifo, uint8_t* buffer, uint16_t size)
{
    // TODO: 在任务2.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t fifo_enqueue(fifo_queue_t* fifo, const uint8_t* data, uint16_t length)
{
    // TODO: 在任务2.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t fifo_dequeue(fifo_queue_t* fifo, uint8_t* data, uint16_t* length)
{
    // TODO: 在任务2.1中实现
    return UART_DMA_ERROR_NONE;
}

static uint16_t fifo_get_count(fifo_queue_t* fifo)
{
    // TODO: 在任务2.1中实现
    return 0;
}

static uart_dma_error_t fifo_flush(fifo_queue_t* fifo)
{
    // TODO: 在任务2.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t ring_buffer_init(ring_buffer_t* ring, uint8_t* buffer, uint16_t size)
{
    // TODO: 在任务3.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t ring_buffer_write(ring_buffer_t* ring, const uint8_t* data, uint16_t length)
{
    // TODO: 在任务3.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t ring_buffer_read(ring_buffer_t* ring, uint8_t* data, uint16_t* length)
{
    // TODO: 在任务3.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t uart_hw_init(uart_hw_config_t* config)
{
    // TODO: 在任务4.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t uart_hw_deinit(void)
{
    // TODO: 在任务4.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_hw_init(uart_dma_hw_config_t* config)
{
    // TODO: 在任务5.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_hw_deinit(void)
{
    // TODO: 在任务5.1中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_start_tx(const uint8_t* data, uint16_t length)
{
    // TODO: 在任务5.2中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_start_rx(uint8_t* buffer, uint16_t length)
{
    // TODO: 在任务5.2中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_stop_tx(void)
{
    // TODO: 在任务5.2中实现
    return UART_DMA_ERROR_NONE;
}

static uart_dma_error_t dma_stop_rx(void)
{
    // TODO: 在任务5.2中实现
    return UART_DMA_ERROR_NONE;
}

// 中断处理函数占位符
void DMA0_Channel1_IRQHandler(void)
{
    // TODO: 在任务5.3中实现
}

void DMA0_Channel2_IRQHandler(void)
{
    // TODO: 在任务5.3中实现
}

void USART0_IRQHandler(void)
{
    // TODO: 在任务4.2中实现
}