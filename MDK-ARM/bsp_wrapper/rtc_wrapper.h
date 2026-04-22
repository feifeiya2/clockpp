/* ======= wrapper_rtc.h ======= */
#ifndef __WRAPPER_RTC_H__
#define __WRAPPER_RTC_H__

#include <stdint.h>
#include <stdbool.h>

// 1. 时间结构体
typedef struct {
    uint16_t year;   // 2026
    uint8_t  month;  // 1-12
    uint8_t  day;    // 1-31
    uint8_t  week;   // 1-7 (周一到周日)
    uint8_t  hour;   // 0-23
    uint8_t  minute; // 0-59
    uint8_t  second; // 0-59
} RTC_Time_t;

typedef struct{
    void (*init)(void);
    bool (*set_time)(const RTC_Time_t *time);
    bool (*get_time)(RTC_Time_t *time);
} RTC_Wrapper_t;

// 2. 提供给上层的方法
void Wrapper_RTC_Init(void);
bool Wrapper_RTC_Set_Time(const RTC_Time_t *time);
bool Wrapper_RTC_Get_Time(RTC_Time_t *time);

// 3. 提供给posting层的注册函数
void Wrapper_RTC_register(RTC_Wrapper_t *wrapper);

#endif /* __WRAPPER_RTC_H__ */
