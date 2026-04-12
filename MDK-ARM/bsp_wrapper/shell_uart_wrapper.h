#ifndef __SHELL_UART_WRAPPER_H__
#define __SHELL_UART_WRAPPER_H__

#include <stdint.h>

typedef struct {
    void (*shell_init)(void);
    void (*shell_send)(uint8_t *data, uint16_t len);
    void (*shell_recv)(uint8_t *data);
} Shell_Uart_Wrapper_t;

//向上层提供的方法
void Wrapper_Shell_Uart_Init(void);
void Wrapper_Shell_Uart_Send(uint8_t *data, uint16_t len);
void Wrapper_Shell_Uart_Recv(uint8_t *data);

//向Posting层提供的注册函数
void Shell_Uart_Register_Wrapper(Shell_Uart_Wrapper_t *wrapper);

#endif /* __SHELL_UART_WRAPPER_H__ */
