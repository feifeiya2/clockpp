#ifndef __SERVICE_NETWORK_H__
#define __SERVICE_NETWORK_H__

#include "stdint.h"
#include "osal.h"

extern osal_task_hdl_t g_Task_Network_Parser_Handle;

void Service_Net_Update_Weather(void);
void Task_Network_Parser(void *argument);
void Service_Net_Init(void);

#endif /* __SERVICE_NETWORK_H__ */
