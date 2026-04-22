#include "osal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "stream_buffer.h"

/* 任务实现 */
osal_status_t osal_task_create(osal_task_hdl_t *pxTask, const char *name, 
                               void (*entry)(void*), void *arg, 
                               uint32_t stack_size, uint8_t priority) {
    // FreeRTOS 使用的是 Word 为单位，这里转为字节
    BaseType_t xRet = xTaskCreate(entry, name, (uint16_t)(stack_size / sizeof(StackType_t)), 
                                  arg, priority, (TaskHandle_t *)pxTask);
    return (xRet == pdPASS) ? OSAL_OK : OSAL_ERROR;
}

void osal_task_delay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void osal_task_delete(osal_task_hdl_t xTask) {
    vTaskDelete((TaskHandle_t)xTask);
}

uint32_t osal_ms_to_tick(uint32_t timeout_ms){
    return timeout_ms / OSAL_TICK_PERIOD_MS;
}

uint32_t osal_get_tick_count(void) {
    return xTaskGetTickCount();
}

/* 互斥锁实现 */
osal_status_t osal_mutex_create(osal_mutex_hdl_t *pxMutex) {
    *pxMutex = (osal_mutex_hdl_t)xSemaphoreCreateMutex();
    return (*pxMutex != NULL) ? OSAL_OK : OSAL_NOMEM;
}

osal_status_t osal_mutex_lock(osal_mutex_hdl_t xMutex, uint32_t timeout_ms) {
    TickType_t ticks = (timeout_ms == OSAL_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return (xSemaphoreTake((SemaphoreHandle_t)xMutex, ticks) == pdPASS) ? OSAL_OK : OSAL_TIMEOUT;
}

osal_status_t osal_mutex_unlock(osal_mutex_hdl_t xMutex) {
    return (xSemaphoreGive((SemaphoreHandle_t)xMutex) == pdPASS) ? OSAL_OK : OSAL_ERROR;
}

void osal_mutex_delete(osal_mutex_hdl_t xMutex) {
    vSemaphoreDelete((SemaphoreHandle_t)xMutex);
}

/* 内存管理直接调用 FreeRTOS 堆管理 */
void* osal_malloc(size_t size) {
    return pvPortMalloc(size);
}

void osal_free(void *ptr) {
    vPortFree(ptr);
}

/* --- Queue 实现 --- */

osal_status_t osal_queue_create(osal_queue_hdl_t *pxQueue, uint32_t queue_len, uint32_t item_size) {
    *pxQueue = (osal_queue_hdl_t)xQueueCreate(queue_len, item_size);
    return (*pxQueue != NULL) ? OSAL_OK : OSAL_NOMEM;
}

osal_status_t osal_queue_send(osal_queue_hdl_t xQueue, const void *item, uint32_t timeout_ms) {
    TickType_t ticks = (timeout_ms == OSAL_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    if (xQueueSend((QueueHandle_t)xQueue, item, ticks) == pdPASS) {
        return OSAL_OK;
    }
    return OSAL_TIMEOUT;
}

osal_status_t osal_queue_receive(osal_queue_hdl_t xQueue, void *item, uint32_t timeout_ms) {
    TickType_t ticks = (timeout_ms == OSAL_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    if (xQueueReceive((QueueHandle_t)xQueue, item, ticks) == pdPASS) {
        return OSAL_OK;
    }
    return OSAL_TIMEOUT;
}

void osal_queue_delete(osal_queue_hdl_t xQueue) {
    vQueueDelete((QueueHandle_t)xQueue);
}

/* --- StreamBuffer 实现 --- */

osal_status_t osal_strm_create(osal_strm_hdl_t *pxStrm, uint32_t buffer_size, uint32_t trigger_level) {
    *pxStrm = (osal_strm_hdl_t)xStreamBufferCreate(buffer_size, trigger_level);
    return (*pxStrm != NULL) ? OSAL_OK : OSAL_NOMEM;
}

size_t osal_strm_send(osal_strm_hdl_t xStrm, const void *data, size_t len, uint32_t timeout_ms) {
    TickType_t ticks = (timeout_ms == OSAL_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xStreamBufferSend((StreamBufferHandle_t)xStrm, data, len, ticks);
}

size_t osal_strm_receive(osal_strm_hdl_t xStrm, void *data, size_t len, uint32_t timeout_ms) {
    TickType_t ticks = (timeout_ms == OSAL_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xStreamBufferReceive((StreamBufferHandle_t)xStrm, data, len, ticks);
}

void osal_strm_delete(osal_strm_hdl_t xStrm) {
    vStreamBufferDelete((StreamBufferHandle_t)xStrm);
}

/* 信号量中断释放 */
osal_status_t osal_sem_give_isr(osal_sem_hdl_t xSem, osal_yield_t *pxYield) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (xSemaphoreGiveFromISR((SemaphoreHandle_t)xSem, &xHigherPriorityTaskWoken) == pdPASS) {
        if (pxYield && xHigherPriorityTaskWoken) *pxYield = OSAL_TRUE;
        return OSAL_OK;
    }
    return OSAL_ERROR;
}

/* 队列中断发送 */
osal_status_t osal_queue_send_isr(osal_queue_hdl_t xQueue, const void *item, osal_yield_t *pxYield) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (xQueueSendFromISR((QueueHandle_t)xQueue, item, &xHigherPriorityTaskWoken) == pdPASS) {
        if (pxYield && xHigherPriorityTaskWoken) *pxYield = OSAL_TRUE;
        return OSAL_OK;
    }
    return OSAL_ERROR;
}

/* 流缓冲中断发送 */
size_t osal_strm_send_isr(osal_strm_hdl_t xStrm, const void *data, size_t len, osal_yield_t *pxYield) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    size_t sent = xStreamBufferSendFromISR((StreamBufferHandle_t)xStrm, data, len, &xHigherPriorityTaskWoken);
    if (pxYield && xHigherPriorityTaskWoken) *pxYield = OSAL_TRUE;
    return sent;
}

/* 强制上下文切换 */
void osal_yield_from_isr(osal_yield_t xYield) {
    portYIELD_FROM_ISR((BaseType_t)xYield);
}
