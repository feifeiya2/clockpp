#include "uart_driver.h"
uint8_t data[100];

void uart_init(void){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, 100);
    
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    printf("uart receive size %d\r\n", Size);

    data[Size] = '\0'; // 添加字符串结束符
    printf("%s", data); // 打印接收到的字符串
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data, 100);
}



