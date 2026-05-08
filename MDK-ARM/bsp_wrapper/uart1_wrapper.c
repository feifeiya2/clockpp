#include "uart1_wrapper.h"
#include "stdio.h"

static uart1_wrapper_t *uart1_wrapper = NULL;

void Wrapper_Uart1_Init(void) {
    Uart1_Posting_Init(); // 初始化Posting层，创建队列等资源
    if(uart1_wrapper == NULL || uart1_wrapper->uart1_init == NULL) {
        printf("Error: UART1 Wrapper not registered or uart1_init not implemented.\n");
        return;
    }
    uart1_wrapper->uart1_init();
}

void Wrapper_Uart1_Send(uint8_t *data, uint16_t len) {
    if(uart1_wrapper == NULL || uart1_wrapper->uart1_send == NULL) {
        printf("Error: UART1 Wrapper not registered or uart1_send not implemented.\n");
        return;
    }
    uart1_wrapper->uart1_send(data, len);
}

void Wrapper_Uart1_Recv(uint8_t *data) {
    if(uart1_wrapper == NULL || uart1_wrapper->uart1_recv == NULL) {
        printf("Error: UART1 Wrapper not registered or uart1_recv not implemented.\n");
        return;
    }
    uart1_wrapper->uart1_recv(data);
}

void Uart1_Register_Wrapper(uart1_wrapper_t *wrapper) {
    uart1_wrapper = wrapper;
}
