#include "elog.h"
#include "osal.h"
#include <stdio.h>
#include "uart1_wrapper.h"

static osal_mutex_hdl_t elog_lock = NULL;
static osal_sem_hdl_t   elog_async_sem = NULL;

/* 声明异步获取函数，防止编译器警告 */
extern size_t elog_async_get_log(char *log, size_t size);

static void elog_async_thread_entry(void *arg) {
    size_t get_log_size = 0;
    static char poll_get_buf[1024];

    while (1) {
        if (osal_sem_take(elog_async_sem, OSAL_WAIT_FOREVER) == OSAL_OK) {
            while (1) {
                /* 注意：这里函数名必须和 elog_async.c 里对应 */
                get_log_size = elog_async_get_log(poll_get_buf, sizeof(poll_get_buf));

                if (get_log_size > 0) {
                    Wrapper_Uart1_Send((uint8_t *)poll_get_buf, (uint16_t)get_log_size);
                } else {
                    break;
                }
            }
        }
    }
}

void elog_async_output_notice(void) {
    if (elog_async_sem != NULL) {
        osal_sem_give(elog_async_sem);
    }
}

ElogErrCode elog_port_init(void) {
    Wrapper_Uart1_Init();
    osal_mutex_create(&elog_lock);
    osal_sem_create(&elog_async_sem, 0);

    osal_task_hdl_t task_hdl;
    /* 创建异步任务 */
    osal_task_create(&task_hdl, "elog_async", elog_async_thread_entry, 
                     NULL, 1024, OSAL_PRIORITYNORMAL - 1);

    return ELOG_NO_ERR;
}

void elog_port_deinit(void) {
    /* 暂时留空 */
}

void elog_port_output(const char *log, size_t size) {
    Wrapper_Uart1_Send((uint8_t *)log, (uint16_t)size);
}

void elog_port_output_lock(void) {
    if (elog_lock != NULL) osal_mutex_lock(elog_lock, OSAL_WAIT_FOREVER);
}

void elog_port_output_unlock(void) {
    if (elog_lock != NULL) osal_mutex_unlock(elog_lock);
}

const char *elog_port_get_time(void) {
    static char cur_time[16];
    snprintf(cur_time, 16, "%u", (unsigned int)osal_get_tick_count());
    return cur_time;
}

const char *elog_port_get_p_info(void) { return "task"; }
const char *elog_port_get_t_info(void) { return ""; }

void EasyLogger_init(void){
          /* 1. EasyLogger 初始化 */
    if (elog_init() == ELOG_NO_ERR) {
        /* 2. 设置日志格式 (这里设置显示所有信息：级别、时间、标签、文件、行号) */
        elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
        elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL);
        elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL);
        elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_TAG & ~ELOG_FMT_P_INFO);
        elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_TAG & ~ELOG_FMT_P_INFO);
        
        /* 3. 启动 EasyLogger */
        elog_start();
        
        /* 4. 开启异步输出模式 (之前在配置里开启了，这里手动使能) */
        elog_async_enabled(true);
    }
}
