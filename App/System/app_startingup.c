#include "app_startingup.h"
#include "osal.h"
#include "app_ui_controller.h"
#include "app_event_bus.h"
#include "service_network.h"


void app_startingup(void){
    EventBus_Init();
    app_ui_controller_init(2048, OSAL_PRIORITYNORMAL);
    osal_task_delay(10);
    Service_Net_Init(1024, OSAL_PRIORITYNORMAL);
}
