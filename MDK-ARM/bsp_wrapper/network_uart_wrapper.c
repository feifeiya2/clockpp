#include "network_uart_wrapper.h"
#include "esp32_uart_posting.h"
#include "string.h"
#include "stdio.h"
#include "osal.h"

static Network_Uart_Wrapper_t *network_uart_wrapper = NULL;

void Wrapper_Network_register(Network_Uart_Wrapper_t *wrapper) {
    network_uart_wrapper = wrapper;
}

static void Wrapper_Network_Uart_Send_AT_Command(const uint8_t *cmd) {
    if(network_uart_wrapper == NULL || network_uart_wrapper->Send_AT_Command == NULL) {
        printf("Error: Network UART Wrapper not registered or Send_AT_Command not implemented.\n");
        return;
    }
    network_uart_wrapper->Send_AT_Command(cmd, strlen((const char *)cmd));
}

/*提供上层的方法 start*/
void Wrapper_Network_Uart_Init(void) {
    ESP32_Uart_Posting_Init();
    if(network_uart_wrapper == NULL || network_uart_wrapper->Init == NULL) {
        printf("Error: Network UART Wrapper not registered or Init not implemented.\n");
        return;
    }
    network_uart_wrapper->Init();
}

uint16_t Wrapper_Network_Uart_Get_Response_Data(uint8_t *buffer, uint16_t max_len) {
    if(network_uart_wrapper == NULL || network_uart_wrapper->Get_Response_Data == NULL) {
        printf("Error: Network UART Wrapper not registered or Get_Response_Data not implemented.\n");
        return 0;
    }
    return network_uart_wrapper->Get_Response_Data(buffer, max_len);
}

/*提供上层的方法 end*/


