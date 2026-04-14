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

/* 对象句柄（使用空指针实现抽象） */
typedef void* osal_task_hdl_t;
typedef void* osal_mutex_hdl_t;
typedef void* osal_sem_hdl_t;
typedef void* osal_queue_hdl_t;

/* 常量定义 */
#define OSAL_WAIT_FOREVER  0xFFFFFFFF
#define OSAL_NO_WAIT       0

#endif /* __OSAL_TYPES_H__ */
