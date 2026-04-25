#include "service_network_hub.h"
#include "cJSON_port.h"
#include <stdio.h>
#include <string.h>
#include "app_event_bus.h"
#include "rtc_wrapper.h"

static WeatherData_t weather_data;

void service_network_hub_init(void) {
    Middleware_cJSON_Init(); // 初始化 cJSON 内存钩子
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
                    
                }
            }
        }
    }

    // 5. 【极其致命】：必须释放内存树，否则立刻导致系统内存泄漏死机！
    cJSON_Delete(root); 
}



/**
 * @brief 解析苏宁时间 JSON 字符串并压入时间队列
 * @note  期待的 JSON 格式片段: {"sysTime1":"20260418151019"}
 */
void DataHub_Parse_Time(const char *json_string) {
    // 1. 生成 cJSON 对象树
    cJSON *root = cJSON_Parse(json_string);
    if (root == NULL) {
        printf("Time cJSON Parse Failed\r\n");
        return;
    }

    // 2. 查找时间字段
    cJSON *time_item = cJSON_GetObjectItem(root, "sysTime1");

    RTC_Time_t syn_rtc_time;

    // 3. 确保找到了，且它是一个字符串
    if (time_item != NULL && cJSON_IsString(time_item)) {
        syn_rtc_time.day = (uint8_t)(time_item->valuestring[6] - '0')*10 + (uint8_t)(time_item->valuestring[7] - '0');
        syn_rtc_time.month = (uint8_t)(time_item->valuestring[4] - '0')*10 + (uint8_t)(time_item->valuestring[5] - '0');
        syn_rtc_time.year = (uint16_t)(time_item->valuestring[0] - '0') * 1000 + (uint16_t)(time_item->valuestring[1] - '0') * 100 + (uint16_t)(time_item->valuestring[2] - '0') * 10 + (uint16_t)(time_item->valuestring[3] - '0');
        syn_rtc_time.hour = (uint8_t)(time_item->valuestring[8] - '0')*10 + (uint8_t)(time_item->valuestring[9] - '0');
        syn_rtc_time.minute = (uint8_t)(time_item->valuestring[10] - '0')*10 + (uint8_t)(time_item->valuestring[11] - '0');
        syn_rtc_time.second = (uint8_t)(time_item->valuestring[12] - '0')*10 + (uint8_t)(time_item->valuestring[13] - '0');
        if(Wrapper_RTC_Set_Time(&syn_rtc_time) == false){
            printf("RTC Set Time Failed\r\n");
        }
    }

    // 5. 【极其致命】：释放内存树！
    cJSON_Delete(root); 
}
