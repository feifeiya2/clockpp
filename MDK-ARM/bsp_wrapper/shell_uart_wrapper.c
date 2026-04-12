#include "shell_uart_wrapper.h"
#include "stdio.h"

static Shell_Uart_Wrapper_t *shell_uart_wrapper = NULL;

void Wrapper_Shell_Uart_Init(void) {
    Shell_Uart_Posting_Init(); // 初始化Posting层，创建队列等资源
    if(shell_uart_wrapper == NULL || shell_uart_wrapper->shell_init == NULL) {
        printf("Error: Shell UART Wrapper not registered or shell_init not implemented.\n");
        return;
    }
    shell_uart_wrapper->shell_init();
}

void Wrapper_Shell_Uart_Send(uint8_t *data, uint16_t len) {
    if(shell_uart_wrapper == NULL || shell_uart_wrapper->shell_send == NULL) {
        printf("Error: Shell UART Wrapper not registered or shell_send not implemented.\n");
        return;
    }
    shell_uart_wrapper->shell_send(data, len);
}

void Wrapper_Shell_Uart_Recv(uint8_t *data) {
    if(shell_uart_wrapper == NULL || shell_uart_wrapper->shell_recv == NULL) {
        printf("Error: Shell UART Wrapper not registered or shell_recv not implemented.\n");
        return;
    }
    shell_uart_wrapper->shell_recv(data);
}

void Shell_Uart_Register_Wrapper(Shell_Uart_Wrapper_t *wrapper) {
    shell_uart_wrapper = wrapper;
}
