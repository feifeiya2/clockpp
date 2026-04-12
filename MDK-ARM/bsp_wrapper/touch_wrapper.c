#include "touch_wrapper.h"
#include "touch_posting.h"
#include "stdio.h"

static Touch_Wrapper_t touch_wrapper;

void Touch_Register_Wrapper(Touch_Wrapper_t *wrapper_interface) {
    touch_wrapper = *wrapper_interface;
}

void Touch_Init(void) {
    Touch_Posting_Init(); // 确保Posting层初始化，注册接口
    if( touch_wrapper.Touch_Init == NULL){
        // TODO: error handling
        return;
    }

    touch_wrapper.Touch_Init();
}

int Touch_Read(Touch_Data_t *state) {
    if( touch_wrapper.Touch_Read == NULL){
        // TODO: error handling
        return -1;
    }

    return touch_wrapper.Touch_Read(state);
}
