#include "uart1_posting.h"
#include "uart1_wrapper.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

static uint8_t uart1_rx_data;
xQueueSetHandle g_uart1_rx_queue;

extern UART_HandleTypeDef huart1;

/* Wrapper层 依赖实现 start*/
static void Port_Uart1_Init(void){
    HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);
}

static void Port_Uart1_Send(uint8_t *data, uint16_t len){
    HAL_UART_Transmit(&huart1, data, len, 100);
}

static void Port_Uart1_Recv(uint8_t *data){
    xQueueReceive(g_uart1_rx_queue, data, portMAX_DELAY);
}
/* Wrapper层 依赖实现 end*/

uart1_wrapper_t uart1_wrapper = {
    .uart1_init = Port_Uart1_Init,
    .uart1_send = Port_Uart1_Send,
    .uart1_recv = Port_Uart1_Recv
};

void Uart1_Posting_Init(void) {
    g_uart1_rx_queue = xQueueCreate(5, sizeof(uint8_t));
    if(g_uart1_rx_queue == NULL) {
        printf("Error: Failed to create UART1 RX queue.\n");
        return;
    }
    Uart1_Register_Wrapper(&uart1_wrapper);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART1){
    xQueueSendFromISR(g_uart1_rx_queue, &uart1_rx_data, NULL);
    HAL_UART_Receive_IT(&huart1, &uart1_rx_data, 1);
  }
}
