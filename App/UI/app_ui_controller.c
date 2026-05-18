#include "app_ui_controller.h"
#include "app_event_bus.h"
#include "service_network_hub.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

osal_task_hdl_t app_ui_controller_task_hdl;

static void app_ui_controller_task(void* argument)
{
    uint32_t temp;

    printf("[UI] task start\r\n");

    for(;;)
    {
        printf("[UI] BEFORE xQueueReceive\r\n");
 
        BaseType_t ret = xQueueReceive(
            (QueueHandle_t)g_app_msg_queue,
            &temp,
            portMAX_DELAY
        );

        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        //printf("[UI] AFTER xQueueReceive, ret=%d, temp=%lu\r\n", ret, temp);
    }
}

// static void app_ui_controller_task(void* argument){
//     App_Event_t event;
//     uint32_t temp;
//     for(;;){
//         //EventBus_Subscribe(&event);
//         //printf("send queue = %p\r\n", g_app_msg_queue);
//         osal_queue_receive(g_app_msg_queue, &temp, OSAL_WAIT_FOREVER);
//         printf("temp = %d\r\n", temp);
//         // printf("run.......................\r\n");
//         // switch(event.id){
//         //     case MSG_WEATHER_UPDATE:
//         //         printf("Weather update\n");
//         //         printf("Weather update: %s\n",((WeatherData_t*)(event.payload.ptr))->weather);
//         //         printf("Temperature: %s\n",((WeatherData_t*)(event.payload.ptr))->temperature);
//         //         break;
//         //     default:
//         //         printf("Unknown event\n");
//         //         break;
//         // }
//     }
// }

void app_ui_controller_init(uint32_t stack_size, uint8_t priority){
    osal_task_create(&app_ui_controller_task_hdl, "app_ui_controller_task", app_ui_controller_task, NULL, stack_size, priority);
}
