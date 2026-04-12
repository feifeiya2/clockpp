#ifndef __ESP32_UART_DRIVER_H__
#define __ESP32_UART_DRIVER_H__

#include "stdint.h"

#define ESP32_RX_BUF_SIZE 2048

/* 1. 定义 ESP32 所需的底层硬件操作集 */
typedef struct {
    void (*send_bytes)(uint8_t *data, uint16_t len); // 发送数据的能力
} ESP32_HW_Ops_t;

/* 2. 驱动层初始化与注册接口 */
void ESP32_Driver_Init(void);
void ESP32_Driver_Register_Ops(ESP32_HW_Ops_t *ops);

/* 3. 数据流向接口 */
void ESP32_Driver_Push_RxData(const uint8_t *data, uint16_t len);     // 给 Posting 中断调用的(存)
uint16_t ESP32_Driver_Pull_RxData(uint8_t *dest, uint16_t max_len);   // 给 Wrapper/App 调用的(取)
void ESP32_Driver_Transmit(const uint8_t *data, uint16_t len);        // 给 Wrapper 调用的(发)

#endif /* __ESP32_UART_DRIVER_H__ */
