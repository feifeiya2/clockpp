#ifndef __OSAL_TYPES_H__
#define __OSAL_TYPES_H__

#include <stdint.h>
#include <stddef.h>

/* 状态返回码 */
typedef enum {
    OSAL_OK       = 0,
    OSAL_ERROR    = -1,
    OSAL_TIMEOUT  = -2,
    OSAL_NOMEM    = -3
} osal_status_t;

typedef enum {
    OSAL_FALSE = 0,
    OSAL_TRUE  = 1
} osal_bool_t;

// 用于记录中断是否触发了高优先级任务唤醒
typedef int32_t osal_yield_t;

/* 对象句柄（使用空指针实现抽象） */
typedef void* osal_task_hdl_t;  // 任务句柄
typedef void* osal_mutex_hdl_t; // 互斥锁句柄
typedef void* osal_sem_hdl_t;   // 信号量句柄
typedef void* osal_queue_hdl_t; // 队列句柄
typedef void* osal_strm_hdl_t;  // StreamBuffer 句柄

/* 常量定义 */
#define OSAL_WAIT_FOREVER       0xFFFFFFFF
#define OSAL_NO_WAIT            0
#define OSAL_TICK_PERIOD_MS     portTICK_PERIOD_MS

#endif /* __OSAL_TYPES_H__ */
