#include "service_network_hub.h"
#include "cJSON_port.h"
#include <stdio.h>
#include <string.h>

osal_queue_hdl_t g_cooked_data_queue_handle; // 完全成熟的数据队列句柄

static WeatherData_t weather_data;

void service_network_hub_init(void) {
    Middleware_cJSON_Init(); // 初始化 cJSON 内存钩子
    osal_queue_create(&g_cooked_data_queue_handle, 5, sizeof(WeatherData_t)); // 创建一个队列用于存储成熟的天气数据
}

/**
 * @brief 解析心知天气 JSON 字符串并存入全局数据池
 * @note  期待的 JSON 格式片段: {"results":[{"now":{"text":"Sunny","temperature":"29"}}]}
 */
void DataHub_Parse_Weather(const char *json_string) {
    // 1. 生成 cJSON 对象树
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        printf("Weather cJSON Parse Failed");
        return;
    }

    // 2. 逐层剥离洋葱找数据
    cJSON *results_arr = cJSON_GetObjectItem(root, "results");
    if (cJSON_IsArray(results_arr)) {
        
        cJSON *first_item = cJSON_GetArrayItem(results_arr, 0); 
        if (first_item != NULL) {
            
            cJSON *now_obj = cJSON_GetObjectItem(first_item, "now");
            if (now_obj != NULL) {
                
                // 3. 提取目标数据项
                cJSON *text_item = cJSON_GetObjectItem(now_obj, "text");
                cJSON *temp_item = cJSON_GetObjectItem(now_obj, "temperature");
                
                if (cJSON_IsString(text_item) && cJSON_IsString(temp_item)) {
                    strcpy(weather_data.weather, text_item->valuestring);
                    strcpy(weather_data.temperature, temp_item->valuestring);
                    if(osal_queue_send(g_cooked_data_queue_handle, &weather_data, 10) == OSAL_TIMEOUT){  // 存入队列
                        printf("Weather Data send to queue timeout\r\n");
                    } 
                }
            }
        }
    }

    // 5. 【极其致命】：必须释放内存树，否则立刻导致系统内存泄漏死机！
    cJSON_Delete(root); 
}

