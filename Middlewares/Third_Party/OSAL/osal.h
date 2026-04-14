#ifndef __OSAL_H__
#define __OSAL_H__

#include "osal_types.h"

/* --- 任务管理 --- */
// stack_size 单位为 字节(Byte)
osal_status_t osal_task_create(osal_task_hdl_t *pxTask, const char *name, 
                               void (*entry)(void*), void *arg, 
                               uint32_t stack_size, uint8_t priority);
void osal_task_delete(osal_task_hdl_t xTask);
void osal_task_delay(uint32_t ms);

/* --- 互斥锁 --- */
osal_status_t osal_mutex_create(osal_mutex_hdl_t *pxMutex);
osal_status_t osal_mutex_lock(osal_mutex_hdl_t xMutex, uint32_t timeout_ms);
osal_status_t osal_mutex_unlock(osal_mutex_hdl_t xMutex);
void osal_mutex_delete(osal_mutex_hdl_t xMutex);

/* --- 信号量 --- */
osal_status_t osal_sem_create(osal_sem_hdl_t *pxSem, uint32_t init_count);
osal_status_t osal_sem_take(osal_sem_hdl_t xSem, uint32_t timeout_ms);
osal_status_t osal_sem_give(osal_sem_hdl_t xSem);
void osal_sem_delete(osal_sem_hdl_t xSem);

/* --- 内存管理 --- */
void* osal_malloc(size_t size);
void  osal_free(void *ptr);

#endif /* __OSAL_H__ */
