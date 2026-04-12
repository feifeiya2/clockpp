#include "esp32_uart_driver.h"
#include "ringbuffer.h"
#include "stdint.h"
#include "stdio.h"

/* ================= 资源私有化 ================= */
// 静态分配专属的 2KB 大内存池
static uint8_t esp32_rb_mem[ESP32_RX_BUF_SIZE];
static RingBuffer esp32_rb;

// 保存外部注入进来的底层硬件操作工具
static ESP32_HW_Ops_t *g_hw_ops = NULL;

/* ================= 核心接口实现 ================= */

void ESP32_Driver_Init(void) {
    // 初始化专属大水库
    RingBuffer_Init(&esp32_rb, esp32_rb_mem, ESP32_RX_BUF_SIZE);
}

void ESP32_Driver_Register_Ops(ESP32_HW_Ops_t *ops) {
    if (ops != NULL) {
        g_hw_ops = ops;
    }
}

// 供 Porting 层的 DMA 空闲中断调用
void ESP32_Driver_Push_RxData(const uint8_t *data, uint16_t len) {
    RingBuffer_Write(&esp32_rb, data, len);
    
    // 【进阶扩展】：因为是专属 ESP32 的驱动，你可以针对网络业务做优化！
    // 比如：在这里发一个 FreeRTOS 信号量 xSemaphoreGiveFromISR()
    // 瞬间唤醒正在死等网络数据的 ESP32_App_Task！
}

// 供 Wrapper 层或网络解析逻辑调用
uint16_t ESP32_Driver_Pull_RxData(uint8_t *dest, uint16_t max_len) {
    return RingBuffer_Read(&esp32_rb, dest, max_len);
}

// 供 Wrapper 层调用（发送 AT 指令或网络报文）
void ESP32_Driver_Transmit(const uint8_t *data, uint16_t len) {
    if (g_hw_ops && g_hw_ops->send_bytes) {
        // 使用被注入的发送工具，盲发数据
        g_hw_ops->send_bytes((uint8_t *)data, len);
    }
}
