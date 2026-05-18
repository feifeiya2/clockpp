#ifndef __SERVICE_NETWORK_HUB_H__
#define __SERVICE_NETWORK_HUB_H__

#include "stdint.h"
#include "osal.h"

typedef struct {
    char weather[10];
    char temperature[5];
} WeatherData_t;

void service_network_hub_init(void);
void DataHub_Parse_Weather(const char *json_string);
void DataHub_Parse_Time(const char *json_string);

#endif /* __SERVICE_NETWORK_HUB_H__ */
