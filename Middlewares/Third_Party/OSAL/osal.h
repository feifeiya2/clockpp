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

/* --- 队列 --- */
// queue_len: 队列深度, item_size: 每个单元的大小(Byte)
osal_status_t osal_queue_create(osal_queue_hdl_t *pxQueue, uint32_t queue_len, uint32_t item_size);
osal_status_t osal_queue_send(osal_queue_hdl_t xQueue, const void *item, uint32_t timeout_ms);
osal_status_t osal_queue_receive(osal_queue_hdl_t xQueue, void *item, uint32_t timeout_ms);
void osal_queue_delete(osal_queue_hdl_t xQueue);


/* --- 流缓冲区 --- */
// buffer_size: 缓冲区总字节数, trigger_level: 唤醒接收任务所需的最小字节数
osal_status_t osal_strm_create(osal_strm_hdl_t *pxStrm, uint32_t buffer_size, uint32_t trigger_level);
size_t osal_strm_send(osal_strm_hdl_t xStrm, const void *data, size_t len, uint32_t timeout_ms);
size_t osal_strm_receive(osal_strm_hdl_t xStrm, void *data, size_t len, uint32_t timeout_ms);
void osal_strm_delete(osal_strm_hdl_t xStrm);


/* --- 中断专用 API --- */

/**
 * @brief 在中断中释放信号量
 * @param pxYield: 必须传入一个 osal_yield_t 变量的指针
 */
osal_status_t osal_sem_give_isr(osal_sem_hdl_t xSem, osal_yield_t *pxYield);

/**
 * @brief 在中断中发送队列消息
 */
osal_status_t osal_queue_send_isr(osal_queue_hdl_t xQueue, const void *item, osal_yield_t *pxYield);

/**
 * @brief 在中断中发送流数据
 */
size_t osal_strm_send_isr(osal_strm_hdl_t xStrm, const void *data, size_t len, osal_yield_t *pxYield);

/**
 * @brief 执行中断上下文切换（在 ISR 退出前调用）
 */
void osal_yield_from_isr(osal_yield_t xYield);

#endif /* __OSAL_H__ */
