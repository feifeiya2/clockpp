#include "service_network.h"
#include "service_network_hub.h"
#include "network_uart_wrapper.h"
#include "stdio.h"
#include "string.h"

void Service_Net_Update_Weather(void){
    Wrapper_Network_Uart_Send_AT_Command((const uint8_t *)"AT+HTTPCGET=\"https://api.seniverse.com/v3/weather/now.json?key=SInj2qoOL5qQE6wJq&location=chengdu&language=en&unit=c\"\r\n");
}

void Service_Net_Update_Time(void){
    Wrapper_Network_Uart_Send_AT_Command((const uint8_t *)"AT+HTTPCGET=\"http://quan.suning.com/getSysTime.do\"\r\n");
}

// 网络解析任务
static void Task_Network_Parser(void *argument) {
    uint8_t rx_line[512]; // 用于存放抠出来的一整行
    
    while(1) {
        uint16_t len = Wrapper_Network_Uart_Get_Complete_Line(rx_line);
        
        if (len > 0) {
            if (strstr((char*)rx_line, "+HTTPCGET:") != NULL) {
                char *json_start = strchr((char*)rx_line, '{');
                if (json_start != NULL) {
                    //天气数据的 JSON 
                    if (strstr(json_start, "\"results\"") != NULL) {
                        DataHub_Parse_Weather(json_start);
                    }
                    //时间数据的 JSON 
                    else if (strstr(json_start, "\"sysTime2\"") != NULL) {
                        DataHub_Parse_Time(json_start);
                    }
                    // 不是我们关心的 JSON
                    else {
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
            // 没收到完整的话，睡 100ms 把 CPU 让给别人
            osal_task_delay(100);
        }
    }
}

osal_task_hdl_t g_Task_Network_Parser_Handle;

void Service_Net_Init(void){
    Wrapper_Network_Uart_Init();
    service_network_hub_init();
    osal_task_create(&g_Task_Network_Parser_Handle, "Network_Parser", Task_Network_Parser, NULL, 1024*4, OSAL_PRIORITYNORMAL);
}
