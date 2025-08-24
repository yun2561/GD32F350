/*!
    \file    Uart.c
    \brief   c file

    \version 2025-06-07, V1.0.0, firmware for GD32F3x0
*/
#include "Uart.h"
#include <stddef.h>

#define uart_0_rcu_periph  RCU_USART0
#define uart_0_periph USART0
#define uart_0_baudrate 115200

#define uart_0_dma_periph USART0;
#define uart_0_dma_tx_ch DMA_CH0
#define uart_0_dma_rx_ch DMA_CH1

#define uart_0_tx_fifo_size 256
#define uart_0_rx_fifo_size 512
#define uart_0_ping_pong_size 128

#define RX_FIFO_SIZE 512
#define TX_FIFO_SIZE 256
#define PING_PONG_SIZE 128

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
//    uart_dma_statistics_t statistics;
//    uart_dma_callback_t tx_callback;
//    uart_dma_callback_t rx_callback;
} uart_system_state_t;

//static uart_system_state_t g_system_state = {0};

static uart_dma_error_t uart_dma_init(uart_dma_config_t* config);

static uart_dma_error_t uart_hw_init(uart_hw_config_t* config);
static uart_dma_error_t uart_dma_hw_init(uart_dma_hw_config_t* config);

static uart_dma_error_t uart_rx_dma_init(uart_dma_hw_config_t* config);
static uart_dma_error_t uart_tx_dma_init(uart_dma_hw_config_t* config);

void UART_Init(void)
{   
    uart_dma_config_t uart0_port_config ={
        .uart_config ={
            .rcu_periph = uart_0_rcu_periph,
            .uart_periph = uart_0_periph,
            .uart_baudrate = uart_0_baudrate,
            .word_lenght = USART_WL_8BIT,
            .stop_bit = USART_PM_NONE,
            .parity = USART_PM_NONE,
            .hardware_flow = NULL,                 
        },
        .dma_config = {
            .dma_periph = DMA,
            .tx_channel = uart_0_dma_tx_ch,
            .rx_channel = uart_0_dma_rx_ch,
            .tx_priority = DMA_PRIORITY_HIGH,
            .rx_priority = DMA_PRIORITY_HIGH,
        },
        /*
        .error_config = {
            .auto_recovery_enabled =TRUE,
            .max_retry_count = 3,
            .recovery_timeout_ms = 1000,
            .error_callback = NULL
        },
        .tx_fifo_size = uart_0_tx_fifo_size,
        .rx_fifo_size = uart_0_rx_fifo_size,
        .ping_pong_size = uart_0_ping_pong_size,
        .enable_circuar_mode = FALSE,
        .enable_error_recovery = TRUE
        */
    };
    uart_dma_init(&uart0_port_config);
     
}

uart_dma_error_t uart_dma_init(uart_dma_config_t* config)
{
    /*//if (config == NULL){
    //    return USRT_DMA_ERROR_INVALID_PARAM;
    //}
    //if (g_system_state.initialized) {
    //    return UART_DMA_ERROR_BUSY;
    //}
    // 保存配置
    //memcpy(&g_system_state.config,config,sizeof(uart_dma_hw_config_t));

    // 初始化FIFO队列
    //fifo_init(&g_fifo_manager.tx_queue, g_fifo_manager.tx_buffer, config->tx_fifo_size);
    //fifo_init(&g_fifo_manager.rx_queue, g_fifo_manager.rx_buffer, config->rx_fifo_size);
    
    // 初始化环形缓冲区
    //ring_buffer_init(&g_ring_manager.tx_ring, g_ring_manager.tx_ping_buffer, config->ping_pong_size);
    //ring_buffer_init(&g_ring_manager.rx_ring, g_ring_manager.rx_ping_buffer, config->ping_pong_size);
    */
    // 初始化硬件
    
    uart_dma_error_t result = uart_hw_init(&config->uart_config);
    if (result != UART_DMA_ERROR_NONE) {
        return result;
    }
    
    result = uart_dma_hw_init(&config->dma_config);
    if (result != UART_DMA_ERROR_NONE) {
        //uart_hw_deinit();
        return result;
    }
    
    return UART_DMA_ERROR_NONE;
}


static uart_dma_error_t uart_hw_init(uart_hw_config_t* config)
{
    // 配置USART参数
    rcu_periph_clock_enable(config->rcu_periph);
    usart_deinit(config->uart_periph);
    usart_baudrate_set(config->uart_periph,config->uart_baudrate);
    usart_parity_config(config->uart_periph,config->parity);
    usart_word_length_set(config->uart_periph,config->word_lenght);
    usart_stop_bit_set(config->uart_periph,config->stop_bit);
    usart_enable(config->uart_periph);
    // 配置USART参数
    return UART_DMA_ERROR_NONE;
}
static uart_dma_error_t uart_dma_hw_init(uart_dma_hw_config_t* config)
{
    
    
    rcu_periph_clock_enable(RCU_DMA);
    
    uart_rx_dma_init(config);
    
    uart_tx_dma_init(config);
    
    return UART_DMA_ERROR_NONE;
    
}

static uart_dma_error_t uart_rx_dma_init(uart_dma_hw_config_t* config)
{
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction=DMA_PERIPHERAL_TO_MEMORY;// 外设到存储器
//    dma_init_struct.memory_addr=(uint32_t)uart0_rx_buffer;// 存储器地址
    dma_init_struct.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width=DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number=RX_FIFO_SIZE;
    dma_init_struct.periph_addr = config->dma_periph; // 外设地址
    dma_init_struct.periph_inc=DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width=DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority=config->rx_priority;
    
    dma_init(config->rx_channel,&dma_init_struct);
    //dam_deinit(config->dma_periph,DMA_CH0);
    dma_circulation_enable(config->rx_channel);
    if(config->rx_enable_circular_mode){
        dma_channel_enable(config->rx_channel);
    }
    else{
        dma_channel_disable(config->rx_channel);
    }
    
    usart_dma_receive_config(config->dma_periph,USART_RECEIVE_DMA_ENABLE);
    
    return UART_DMA_ERROR_NONE;
    
}
static uart_dma_error_t uart_tx_dma_init(uart_dma_hw_config_t* config)
{
    dma_parameter_struct dma_init_struct;
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction=DMA_MEMORY_TO_PERIPHERAL ;// 外设到存储器
//    dma_init_struct.memory_addr=(uint32_t)uart0_tx_buffer;// 存储器地址
    dma_init_struct.memory_inc=DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width=DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number=0;
    dma_init_struct.periph_addr = config->dma_periph; // 外设地址
    dma_init_struct.periph_inc=DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width=DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority=config->tx_priority;
    
    dma_init(config->tx_channel,&dma_init_struct);
    
    if(config->rx_enable_circular_mode){
        dma_circulation_enable(config->tx_channel);
    }
    else{
        dma_circulation_disable(config->tx_channel);
    }
    
    dma_channel_enable(config->tx_channel);
    
    usart_dma_transmit_config(config->dma_periph,USART_TRANSMIT_DMA_ENABLE);
    
    return UART_DMA_ERROR_NONE;
    
}
/*
uart_dma_error_t uart_dma_send(const uint8_t* data, uint16_t length)
{
    // TODO: 在任务8.2中实现
    return UART_DMA_ERROR_NOT_INITIALIZED;
    //return 0;
}

//uart_dma_error_t uart_dma_send_async(const uint8_t* data, uint16_t length, uart_dma_callback_t callback)
//{
    // TODO: 在任务8.2中实现
    //return UART_DMA_ERROR_NOT_INITIALIZED;
//    return 0;
//}

// 接收接口占位符
uart_dma_error_t uart_dma_receive(uint8_t* buffer, uint16_t length)
{
    // TODO: 在任务8.3中实现
    //return UART_DMA_ERROR_NOT_INITIALIZED;
    return 0;
}

//uart_dma_error_t uart_dma_receive_async(uint8_t* buffer, uint16_t length, uart_dma_callback_t callback)
//{
    // TODO: 在任务8.3中实现
    //return UART_DMA_ERROR_NOT_INITIALIZED;
//    return 0;
//}

// 状态查询接口占位符
uart_dma_status_t uart_dma_get_tx_status(void)
{
//    return g_system_state.tx_status;
    return 0;
}

uart_dma_status_t uart_dma_get_rx_status(void)
{
    //return g_system_state.rx_status;
    return 0;
}
*/
