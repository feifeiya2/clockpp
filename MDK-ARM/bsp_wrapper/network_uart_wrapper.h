#ifndef __NETWORK_UART_WRAPPER_H__
#define __NETWORK_UART_WRAPPER_H__

#include <stdint.h>

typedef struct {
    void (*Init)(void);
    void (*Send_AT_Command)(const uint8_t *cmd, uint16_t len);
    uint16_t (*Get_Complete_Line)(uint8_t *buffer);
} Network_Uart_Wrapper_t;

//提供给上层的接口函数声明
void Wrapper_Network_Uart_Init(void);
uint16_t Wrapper_Network_Uart_Get_Complete_Line(uint8_t *buffer);
void Wrapper_Network_Uart_Send_AT_Command(const uint8_t *cmd);

//提供给posting层的注册函数声明
void Wrapper_Network_register(Network_Uart_Wrapper_t *wrapper);

#endif /* __NETWORK_UART_WRAPPER_H__ */
