#ifndef __SERVICE_NETWORK_HUB_H__
#define __SERVICE_NETWORK_HUB_H__

#include "stdint.h"
#include "osal.h"

typedef struct {
    char weather[10];
    char temperature[5];
} WeatherData_t;
extern osal_queue_hdl_t g_cooked_data_queue_handle;

void service_network_hub_init(void);
void DataHub_Parse_Weather(const char *json_string);

#endif /* __SERVICE_NETWORK_HUB_H__ */
