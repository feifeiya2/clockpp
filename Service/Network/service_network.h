#ifndef __SERVICE_NETWORK_H__
#define __SERVICE_NETWORK_H__

#include "stdint.h"
#include "osal.h"

extern osal_strm_hdl_t g_Strm_Time_data_Handle;

void Service_Net_Update_Weather(void);
void Service_Net_Update_Time(void);
void Service_Net_Init(uint32_t stack_size, uint8_t priority);

#endif /* __SERVICE_NETWORK_H__ */
