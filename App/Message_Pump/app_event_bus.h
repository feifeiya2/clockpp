#ifndef __APP_EVENT_BUS_H__
#define __APP_EVENT_BUS_H__

#include "osal.h"
#include <stdbool.h>

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

// 3. 对外暴露的方法
void EventBus_Init(void);
bool EventBus_Publish(App_Msg_ID_e id, void *ptr, uint32_t val); // 供 Service 层调用
bool EventBus_Subscribe(App_Event_t *out_event);  // 供 App 层调用

extern osal_queue_hdl_t g_app_msg_queue;   //消息队列

#endif   // __APP_EVENT_BUS_H__
