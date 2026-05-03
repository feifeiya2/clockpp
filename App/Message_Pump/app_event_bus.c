#include "app_event_bus.h"
#include <stdio.h>


App_Event_t g_app_msg;                //消息泵
osal_queue_hdl_t g_app_msg_queue;   //消息队列

void EventBus_Init(void){
    //osal_queue_create(&g_app_msg_queue, 5, sizeof(App_Event_t));
    osal_queue_create(&g_app_msg_queue, 5, sizeof(uint32_t));
}

bool EventBus_Publish(App_Msg_ID_e id, void *ptr, uint32_t val){
    // if (g_app_msg_queue == NULL) return false;
    printf("here publish\r\n");
    // App_Event_t evt;
    // evt.id = id;
    // evt.payload.ptr = ptr;
    // evt.payload.val = val;
    // return (osal_queue_send(g_app_msg_queue, &evt, 0) == OSAL_OK);
    uint32_t t = 1;
    //printf("send queue = %p\r\n", g_app_msg_queue);
    return (osal_queue_send(g_app_msg_queue, &t, 100) == OSAL_OK);
}

// 订阅/获取消息 (消费者调用)
bool EventBus_Subscribe(App_Event_t *out_event) {
    if (g_app_msg_queue == NULL) return false;
    printf("here subscribe\r\n");
    osal_queue_receive(g_app_msg_queue, out_event, OSAL_WAIT_FOREVER);
    return true;
}
