#ifndef __APP_UI_CONTROLLER_H__
#define __APP_UI_CONTROLLER_H__

#include "osal.h"

extern osal_task_hdl_t app_ui_controller_task_hdl;
void app_ui_controller_init(uint32_t stack_size, uint8_t priority);

#endif // __APP_UI_CONTROLLER_H__
