#include "osal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

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

/* 信号量实现参考互斥锁，篇幅原因略去... */
