#include "cJSON_port.h"

/* ======= service_network.c 或任意系统启动位置 ======= */
#include "cJSON.h"
#include "osal.h"

// 专门为 cJSON 准备的初始化函数
void Middleware_cJSON_Init(void) {
    // 1. 定义一个 cJSON 的钩子结构体
    cJSON_Hooks hooks;
    
    // 2. 将钩子指向 FreeRTOS 强大的内存管理函数
    hooks.malloc_fn = osal_malloc;
    hooks.free_fn   = osal_free;
    
    // 3. 把这套钩子打入 cJSON 的心脏！
    // 从此以后，cJSON 会极其安全地从 FreeRTOS 的大堆里借用内存。
    cJSON_InitHooks(&hooks);
}
