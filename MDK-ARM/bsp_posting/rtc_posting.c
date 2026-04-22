#include "rtc_posting.h"
#include "rtc_wrapper.h"
#include "main.h"
#include <stdbool.h>

extern RTC_HandleTypeDef hrtc;

/* Wrapper 层的接口实现 start */
/**
 * @brief  向硬件写入时间
 */
static bool Port_RTC_Set_Time(const RTC_Time_t *sys_time) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // 1. 翻译时间
    sTime.Hours   = sys_time->hour;
    sTime.Minutes = sys_time->minute;
    sTime.Seconds = sys_time->second;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
        return false;
    }

    // 2. 翻译日期
    sDate.WeekDay = sys_time->week;
    sDate.Month   = sys_time->month;
    sDate.Date    = sys_time->day;
    // STM32 硬件里的 Year 只能存 0~99 所以必须减去 2000
    sDate.Year    = (uint8_t)(sys_time->year - 2000);

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
        return false;
    }
    
    // 设备掉电重启， RTC重置凭证
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x5A4A);
    
    return true;
}

/**
 * @brief  从硬件读取时间
 */
static bool Port_RTC_Get_Time(RTC_Time_t *sys_time) {
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    // 调用 HAL 库读取, 必须先读 Time，再读 DateSTM32 硬件手册死规定
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) return false;
    if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) return false;

    sys_time->hour   = sTime.Hours;
    sys_time->minute = sTime.Minutes;
    sys_time->second = sTime.Seconds;
    
    sys_time->week  = sDate.WeekDay;
    sys_time->month = sDate.Month;
    sys_time->day   = sDate.Date;
    sys_time->year  = sDate.Year + 2000;

    return true;
}

static void Port_RTC_Init(void) {
    if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1)!= 0x5A4A){
        // 设备第一次上电，或者掉电重启了，RTC寄存器被重置了
        // 这里可以做一些初始化，比如设置默认时间，或者从外部存储读取时间
        RTC_Time_t default_time = {2026, 1, 1, 4, 0, 0, 0}; // 默认时间：2026-01-01 00:00:00
        Port_RTC_Set_Time(&default_time);
    }
}
/* Wrapper 层的接口实现 end */

static RTC_Wrapper_t rtc_wrapper = {
    .init = Port_RTC_Init,
    .set_time = Port_RTC_Set_Time,
    .get_time = Port_RTC_Get_Time
};

void RTC_Porting_Init(void) {
    // 注册到 Wrapper 层
    Wrapper_RTC_register(&rtc_wrapper);
}
