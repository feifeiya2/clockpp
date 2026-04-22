#include "rtc_wrapper.h"
#include "stdio.h"
#include "rtc_posting.h"

static RTC_Wrapper_t *rtc_wrapper = NULL;

void Wrapper_RTC_Init(void){
    RTC_Porting_Init();
    if(rtc_wrapper == NULL || rtc_wrapper->init == NULL) {
        printf("Error: RTC Wrapper not registered or init not implemented.\n");
        return;
    }
    rtc_wrapper->init();
}

bool Wrapper_RTC_Set_Time(const RTC_Time_t *time){
    if(rtc_wrapper == NULL || rtc_wrapper->set_time == NULL) {
        printf("Error: RTC Wrapper not registered or set_time not implemented.\n");
        return false;
    }
    return rtc_wrapper->set_time(time);
}

bool Wrapper_RTC_Get_Time(RTC_Time_t *time){
    if(rtc_wrapper == NULL || rtc_wrapper->get_time == NULL) {
        printf("Error: RTC Wrapper not registered or get_time not implemented.\n");
        return false;
    }
    return rtc_wrapper->get_time(time);
}

void Wrapper_RTC_register(RTC_Wrapper_t *wrapper) {
    rtc_wrapper = wrapper;
}
