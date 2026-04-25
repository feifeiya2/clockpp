#ifndef __APP_EVENT_BUS_H__
#define __APP_EVENT_BUS_H__

#include "osal.h"

typedef enum {
    MSG_WEATHER_UPDATE,
    MSG_TIME_UPDATE
} App_Msg_ID_e;

// 2. 消息载体
typedef struct {
    App_Msg_ID_e id;
    union {
        void *   ptr;   // 用于传字符串或大对象的指针
        uint32_t val;   // 用于传整数
    } payload;
} App_Event_t;

// 声明全局邮箱和全局LVGL锁
extern osal_queue_hdl_t g_app_msg_queue;

#endif   // __APP_EVENT_BUS_H__