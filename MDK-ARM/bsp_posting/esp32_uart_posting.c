#include "esp32_uart_posting.h"
#include "stdio.h"
#include "string.h"
#include "network_uart_wrapper.h"
#include "esp32_uart_driver.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
static uint8_t esp_uart_buffer[1024];

static void start_uart_dma_receive(void) {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, esp_uart_buffer, sizeof(esp_uart_buffer));
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
}

/* driver层 依赖实现 start */
static void Port_ESP32_Send(uint8_t *data, uint16_t len) {
    HAL_UART_Transmit_DMA(&huart2, data, len);
}
/* driver层 依赖实现 end */


/* wrapper层 依赖实现 start */
static void Port_Network_Init(void){
    ESP32_Driver_Init();
    start_uart_dma_receive();
}

static void Port_Network_Uart_Send_AT_Command(const uint8_t *cmd, uint16_t len){
    ESP32_Driver_Transmit(cmd, len);
}

static uint16_t Port_Network_Get_Complete_Line(uint8_t *out_line) {
    
    // 1. 【开启雷达扫描】：问 Driver 层，水库里现在有没有完整的一行？有多长？
    uint16_t line_len = ESP32_Driver_Get_Complete_datalen();
    // 2. 判断扫描结果
    if (line_len > 0) {
        ESP32_Driver_Pull_RxData(out_line, line_len);
        // 封口变成标准字符串
        out_line[line_len] = '\0'; 
        return line_len; // 完美返回    
    }
    return 0; 
}
/* wrapper层 依赖实现 end */

static Network_Uart_Wrapper_t network_uart_wrapper = {
    .Init = Port_Network_Init,
    .Send_AT_Command = Port_Network_Uart_Send_AT_Command,
    .Get_Complete_Line = Port_Network_Get_Complete_Line
};
static ESP32_HW_Ops_t esp_hw_ops = { .send_bytes = Port_ESP32_Send };


void ESP32_Uart_Posting_Init(void) {
    ESP32_Driver_Register_Ops(&esp_hw_ops);
    Wrapper_Network_register(&network_uart_wrapper);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance == USART2){
        ESP32_Driver_Push_RxData(esp_uart_buffer, Size);
        start_uart_dma_receive();
    }
}
