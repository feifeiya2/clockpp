#ifndef __UART1_WRAPPER_H__
#define __UART1_WRAPPER_H__

#include <stdint.h>

typedef struct {
    void (*uart1_init)(void);
    void (*uart1_send)(uint8_t *data, uint16_t len);
    void (*uart1_recv)(uint8_t *data);
} uart1_wrapper_t;

//向上层提供的方法
void Wrapper_Uart1_Init(void);
void Wrapper_Uart1_Send(uint8_t *data, uint16_t len);
void Wrapper_Uart1_Recv(uint8_t *data);

//向Posting层提供的注册函数
void Uart1_Register_Wrapper(uart1_wrapper_t *wrapper);

#endif /* __UART1_WRAPPER_H__ */
