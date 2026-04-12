#include "shell_uart_posting.h"
#include "shell_uart_wrapper.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

static uint8_t shell_rx_data;
xQueueSetHandle g_shell_rx_queue;

extern UART_HandleTypeDef huart1;

/* Wrapper层 依赖实现 start*/
static void Port_Shell_Uart_Init(void){
    HAL_UART_Receive_IT(&huart1, &shell_rx_data, 1);
}

static void Port_Shell_Uart_Send(uint8_t *data, uint16_t len){
    HAL_UART_Transmit(&huart1, data, len, 100);
}

static void Port_Shell_Uart_Recv(uint8_t *data){
    xQueueReceive(g_shell_rx_queue, data, portMAX_DELAY);
}
/* Wrapper层 依赖实现 end*/

Shell_Uart_Wrapper_t shell_uart_wrapper = {
    .shell_init = Port_Shell_Uart_Init,
    .shell_send = Port_Shell_Uart_Send,
    .shell_recv = Port_Shell_Uart_Recv
};

void Shell_Uart_Posting_Init(void) {
    g_shell_rx_queue = xQueueCreate(5, sizeof(uint8_t));
    if(g_shell_rx_queue == NULL) {
        printf("Error: Failed to create shell RX queue.\n");
        return;
    }
    Shell_Uart_Register_Wrapper(&shell_uart_wrapper);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1){
    xQueueSendFromISR(g_shell_rx_queue, &shell_rx_data, NULL);
    HAL_UART_Receive_IT(&huart1, &shell_rx_data, 1);
  }
}
