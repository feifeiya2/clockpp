#include "service_network.h"
#include "network_uart_wrapper.h"
#include "stdio.h"
#include "string.h"

void Service_Net_Update_Weather(void){
    Wrapper_Network_Uart_Send_AT_Command((const uint8_t *)"AT+HTTPCGET=\"https://api.seniverse.com/v3/weather/now.json?key=SInj2qoOL5qQE6wJq&location=chengdu&language=en&unit=c\"\r\n");
}

// 网络解析任务
void Task_Network_Parser(void *argument) {
    uint8_t rx_line[512]; // 用于存放抠出来的一整行
    
    while(1) {
        uint16_t len = Wrapper_Network_Uart_Get_Complete_Line(rx_line);
        
        if (len > 0) {
            if (strstr((char*)rx_line, "+HTTPCGET:") != NULL) {
                char *json_start = strchr((char*)rx_line, '{');
                if (json_start != NULL) {
                    if (strstr(json_start, "\"results\"") != NULL) {
                        // 确认完毕！这就是天气数据！
                        // 立刻把它丢给 DataHub 去做真正的 cJSON 提取！
                        printf("Received Weather Data: %s\n", json_start);
                        
                    } else {
                        // 不是我们关心的 JSON，先打印出来看看
                        printf("Received HTTP Response: %s\n", json_start);
                    }
                } else {
                    printf("Malformed HTTP Response: %s\n", rx_line);
                }
            }

            else {
                printf("AT Reply: %s\n", rx_line);
            }
        }
        else {
            // 没收到完整的话，睡 10ms 把 CPU 让给别人
            vTaskDelay(10);
        }
    }
}

osal_task_hdl_t g_Task_Network_Parser_Handle;

void Service_Net_Init(void){
    Wrapper_Network_Uart_Init();
    osal_task_create(&g_Task_Network_Parser_Handle, "Network_Parser", Task_Network_Parser, NULL, 1024*4, OSAL_PRIORITYNORMAL);
}
